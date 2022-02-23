#ifndef __ARM_CORTEX_M4_DWT_HH__
#define __ARM_CORTEX_M4_DWT_HH__

#include "params/ARMDWT.hh"
#include "sim/sim_object.hh"

namespace ARMCortexM4
{

class DWT: public gem5::SimObject
{
  public:
    DWT(const gem5::ARMDWTParams &params);
    ~DWT();
};

} // namespace ARMCortexM4

#endif // __ARM_CORTEX_M4_DWT_HH__