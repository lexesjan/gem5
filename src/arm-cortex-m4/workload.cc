// Adapted from https://github.com/devplayer0/gem5/blob/perfgrade/src/perfgrade/workload.cc

#include "arm-cortex-m4/workload.hh"

#include "arch/arm/faults.hh"
#include "base/loader/raw_image.hh"
#include "debug/Loader.hh"
#include "params/ArmROMWorkload.hh"
#include "sim/byteswap.hh"
#include "sim/system.hh"

namespace ArmCortexM4 {

ArmROMWorkload::ArmROMWorkload(const Params &p) : StubWorkload(p), _params(p)
{
    if (params().rom_file == "") {
        fatal("No ROM file set for full system simulation");
    }

    gem5::loader::ImageFileDataPtr ifd(
        new gem5::loader::ImageFileData(params().rom_file));
    image = gem5::loader::RawImage(ifd).buildImage();

    _start = 0x08000000 + image.minAddr();
    _end   = 0x08000000 + image.maxAddr();

    // Need to read the ROM directly to get the entrypoint early on
    entry = gem5::letoh(((uint32_t*)ifd->data())[1]);

    gem5::loader::Symbol ep;
    ep.binding = gem5::loader::Symbol::Binding::Global;
    ep.name = "_start";
    ep.address = entry & ~1;

    insertSymbol(ep);
    gem5::loader::debugSymbolTable.insert(_symtab);
}

void
ArmCortexM4::ArmROMWorkload::initState()
{
    auto &phys_mem = system->physProxy;

    // ROM exists at 0x00000000 and at 0x08000000
    image.write(phys_mem);

    image.offset(0x08000000);
    image.write(phys_mem);

    gem5::RegVal initial_sp = phys_mem.read<uint32_t>(0x0);

    auto *t0 = system->threads[0];

    gem5::ArmISA::Reset().invoke(t0);
    t0->activate();

    t0->setIntReg(gem5::ArmISA::StackPointerReg, initial_sp);

    // Make sure the T flag is set
    gem5::ArmISA::CPSR cpsr = t0->readMiscRegNoEffect(
        gem5::ArmISA::MISCREG_CPSR);
    cpsr.t = 1;
    t0->setMiscRegNoEffect(gem5::ArmISA::MISCREG_CPSR, cpsr);

    // For gem5 to be happy the PC must be aligned, changing the pcState with
    // the CPSR T flag set will unalign it
    gem5::ArmISA::PCState pc = t0->pcState().as<gem5::ArmISA::PCState>();
    pc.thumb(true);
    pc.nextThumb(true);
    pc.set(pc.pc() & ~gem5::mask(1));
    t0->pcState(pc);
}

} // namespace ArmCortexM4