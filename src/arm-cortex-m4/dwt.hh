#ifndef __ARM_CORTEX_M4_DWT_HH__
#define __ARM_CORTEX_M4_DWT_HH__

#include "dev/io_device.hh"
#include "params/ArmDWT.hh"

namespace ArmCortexM4
{

class DWT: public gem5::BasicPioDevice
{
  public:
    DWT(const gem5::ArmDWTParams &params);
    ~DWT();

    /**
     * Handle a read to the device
     * @param pkt The memory request.
     * @param data Where to put the data.
     */
    gem5::Tick read(gem5::PacketPtr pkt) override;

    /**
     * Handle a write to the device
     * @param pkt The memory request.
     * @param data the data
     */
    gem5::Tick write(gem5::PacketPtr pkt) override;
};

} // namespace ArmCortexM4

#endif // __ARM_CORTEX_M4_DWT_HH__