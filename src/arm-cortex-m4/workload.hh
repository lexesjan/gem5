// Adapted from https://github.com/devplayer0/gem5/blob/perfgrade/src/perfgrade/workload.hh

#ifndef __ARMROM_WORKLOAD_HH__
#define __ARMROM_WORKLOAD_HH__

#include <string>
#include <vector>

#include "base/loader/symtab.hh"
#include "base/types.hh"
#include "params/ARMROMWorkload.hh"
#include "sim/workload.hh"

class System;

namespace ARMCortexM4 {

class ARMROMWorkload : public Workload
{
  public:
    using Params = ARMROMWorkloadParams;

  protected:
    const Params &_params;

    Loader::MemoryImage image;
    Addr entry;

    Addr _start, _end;
    Loader::SymbolTable _symtab;

  public:
    const Params &params() const { return _params; }

    Addr start() const { return _start; }
    Addr end() const { return _end; }

    ARMROMWorkload(const Params &p);

    Addr getEntry() const override { return entry; }
    Loader::Arch
    getArch() const override
    {
        return Loader::Thumb;
    }

    const Loader::SymbolTable &
    symtab(ThreadContext *tc) override
    {
        return _symtab;
    }

    bool
    insertSymbol(const Loader::Symbol &symbol) override
    {
        return _symtab.insert(symbol);
    }

    void initState() override;

    void serialize(CheckpointOut &cp) const override;
    void unserialize(CheckpointIn &cp) override;
};

} // namespace ARMCortexM4

#endif // __ARMROM_WORKLOAD_HH__