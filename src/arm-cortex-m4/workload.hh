// Adapted from https://github.com/devplayer0/gem5/blob/perfgrade/src/perfgrade/workload.hh

#ifndef __ARM_CORTEX_M4_ARMROM_WORKLOAD_HH__
#define __ARM_CORTEX_M4_ARMROM_WORKLOAD_HH__

#include <string>
#include <vector>

#include "arch/arm/remote_gdb.hh"
#include "base/loader/symtab.hh"
#include "base/types.hh"
#include "params/ArmROMWorkload.hh"
#include "sim/workload.hh"

class System;

namespace ArmCortexM4 {

class ArmROMWorkload : public gem5::StubWorkload
{
  public:
    using Params = gem5::ArmROMWorkloadParams;

  protected:
    const Params &_params;

    gem5::loader::MemoryImage image;
    gem5::Addr entry;

    gem5::Addr _start, _end;
    gem5::loader::SymbolTable _symtab;

  public:
    const Params &params() const { return _params; }

    gem5::Addr start() const { return _start; }
    gem5::Addr end() const { return _end; }

    ArmROMWorkload(const Params &p);

    gem5::Addr getEntry() const override { return entry; }
    gem5::loader::Arch
    getArch() const override
    {
        return gem5::loader::Thumb;
    }

    void
    setSystem(gem5::System *sys) override
    {
        gem5::Workload::setSystem(sys);
        gdb = gem5::BaseRemoteGDB::build<gem5::ArmISA::RemoteGDB>(system);
    }

    const gem5::loader::SymbolTable &
    symtab(gem5::ThreadContext *tc) override
    {
        return _symtab;
    }

    bool
    insertSymbol(const gem5::loader::Symbol &symbol) override
    {
        return _symtab.insert(symbol);
    }

    void initState() override;
};

} // namespace ArmCortexM4

#endif // __ARM_CORTEX_M4_ARMROM_WORKLOAD_HH__