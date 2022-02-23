#include "arm-cortex-m4/dwt.hh"

namespace ARMCortexM4
{

DWT::DWT(const gem5::ARMDWTParams &params) : SimObject(params)
{
}

DWT::~DWT()
{
}

} // namespace ARMCortexM4