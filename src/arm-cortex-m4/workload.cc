// Adapted from https://github.com/devplayer0/gem5/blob/perfgrade/src/perfgrade/workload.cc

#include "arm-cortex-m4/workload.hh"

#include "arch/arm/faults.hh"
#include "arch/arm/registers.hh"
#include "base/loader/raw_image.hh"
#include "debug/Loader.hh"
#include "params/ARMROMWorkload.hh"
#include "sim/byteswap.hh"
#include "sim/system.hh"

namespace ARMCortexM4 {

ARMROMWorkload::ARMROMWorkload(const Params &p) : Workload(p), _params(p)
{
    if (params().rom_file == "") {
        fatal("No ROM file set for full system simulation");
    }

    Loader::ImageFileDataPtr ifd(new Loader::ImageFileData(params().rom_file));
    image = Loader::RawImage(ifd).buildImage();

    _start = 0x08000000 + image.minAddr();
    _end   = 0x08000000 + image.maxAddr();

    // Need to read the ROM directly to get the entrypoint early on
    entry = letoh(((uint32_t*)ifd->data())[1]);

    Loader::Symbol ep;
    ep.binding = Loader::Symbol::Binding::Global;
    ep.name = "_start";
    ep.address = entry & ~1;

    insertSymbol(ep);
    Loader::debugSymbolTable.insert(_symtab);
}

void
ARMCortexM4::ARMROMWorkload::initState()
{
    auto &phys_mem = system->physProxy;

    // ROM exists at 0x00000000 and at 0x08000000
    image.write(phys_mem);

    image.offset(0x08000000);
    image.write(phys_mem);

    // Magic @ CPUID register
    phys_mem.write(0xe000ed00, 0xcafebabe);

    RegVal initial_sp = phys_mem.read<uint32_t>(0x0);

    DPRINTF(Loader, "Initial SP value = %#x\n", initial_sp);
    DPRINTF(Loader, "Entry point      = %#x\n", getEntry());

    auto *t0 = system->threads[0];

    ArmISA::Reset().invoke(t0);
    t0->activate();

    t0->setIntReg(ArmISA::StackPointerReg, initial_sp);

    // Make sure the T flag is set
    ArmISA::CPSR cpsr = t0->readMiscRegNoEffect(ArmISA::MISCREG_CPSR);
    cpsr.t = 1;
    t0->setMiscRegNoEffect(ArmISA::MISCREG_CPSR, cpsr);

    // For gem5 to be happy the PC must be aligned, changing the pcState with
    // the CPSR T flag set will unalign it
    ArmISA::PCState pc = t0->pcState();
    pc.thumb(true);
    pc.nextThumb(true);
    pc.set(pc.pc() & ~mask(1));
    t0->pcState(pc);
}

void
ARMCortexM4::ARMROMWorkload::serialize(CheckpointOut &cp) const
{
    _symtab.serialize("symtab", cp);
}

void
ARMCortexM4::ARMROMWorkload::unserialize(CheckpointIn &cp)
{
    _symtab.unserialize("symtab", cp);
}

} // namespace ARMCortexM4