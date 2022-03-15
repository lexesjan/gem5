#ifndef __ARM_PERFORMANCE_DWT_HH__
#define __ARM_PERFORMANCE_DWT_HH__

#include <vector>

#include "base/bitunion.hh"
#include "dev/io_device.hh"
#include "params/ArmDWT.hh"

namespace ArmPerformance
{

class DWT: public gem5::BasicPioDevice
{
  public:
    DWT(const gem5::ArmDWTParams &params);
    ~DWT();

    enum
    {
        DWT_CTRL      = 0x0,
        DWT_CYCCNT    = 0x4,
        DWT_CPICNT    = 0x8,
        DWT_EXCCNT    = 0xC,
        DWT_SLEEPCNT  = 0x10,
        DWT_LSUCNT    = 0x14,
        DWT_FOLDCNT   = 0x18,
        DWT_PCSR      = 0x1C,
        DWT_COMP0     = 0x20,
        DWT_MASK0     = 0x24,
        DWT_FUNCTION0 = 0x28,
        DWT_COMP1     = 0x30,
        DWT_MASK1     = 0x34,
        DWT_FUNCTION1 = 0x38,
        DWT_COMP2     = 0x40,
        DWT_MASK2	    = 0x44,
        DWT_FUNCTION2 = 0x48,
        DWT_COMP3     = 0x50,
        DWT_MASK3     = 0x54,
        DWT_FUNCTION3 = 0x58,
        PID4          = 0x1FD0,
        PID5          = 0x1FD4,
        PID6          = 0x1FD8,
        PID7          = 0x1FDC,
        PID0          = 0x1FE0,
        PID1          = 0x1FE4,
        PID2          = 0x1FE8,
        PID3          = 0x1FEC,
        CID0          = 0x1FF0,
        CID1          = 0x1FF4,
        CID2          = 0x1FF8,
        CID3          = 0x1FFC,

        DWT_NCNTRS     = 0x6,
    };

    BitUnion32(DWT_CTRL_t)
        Bitfield<0>     CYCCNTENA;
        Bitfield<4,1>   POSTPRESET;
        Bitfield<8,5>   POSTINIT;
        Bitfield<9>     CYCTAP;
        Bitfield<11,10> SYNCTAP;
        Bitfield<12>    PCSAMPLENA;
        Bitfield<16>    EXCTRCENA;
        Bitfield<17>    CPIEVTENA;
        Bitfield<18>    EXCEVTENA;
        Bitfield<19>    SLEEPEVTENA;
        Bitfield<20>    LSUEVTENA;
        Bitfield<21>    FOLDEVTENA;
        Bitfield<22>    CYCEVTENA;
        Bitfield<24>    NOPRFCNT;
        Bitfield<25>    NOCYCCNT;
        Bitfield<26>    NOEXTTRIG;
        Bitfield<27>    NOTRCPKT;
        Bitfield<31,28> NUMCOMP;
    EndBitUnion(DWT_CTRL_t)

    /**
     * Increments a counter by the value
     *
     * @param offsetAddr ID of the counter.
     * @param value Value to increment by.
     */
    void incrementCounterValue(uint32_t offsetAddr, uint32_t value);

  private:
    /** Control register */
    DWT_CTRL_t control;

    /** Current state of the counters */
    std::vector<uint32_t> counters;

    /**
     * Checks if the counter register address is valid
     *
     * @param registerAddr Register address to check.
     */
    bool isValidCounterRegisterAddr(uint32_t registerAddr)
    {
        return registerAddr % 4 == 0 && registerAddr >= DWT_CYCCNT &&
               registerAddr <= DWT_FOLDCNT;
    }

    /**
     * Converts a register address to a counter ID
     *
     * @param registerAddr Offset address to convert.
     */
    uint32_t registerAddrToId(uint32_t registerAddr)
    {
      return registerAddr / static_cast<uint32_t>(DWT_NCNTRS);
    }

  // BasicPioDevice
  public:
    /**
     * Handle a read to the device
     *
     * @param pkt The memory request.
     * @param data Where to put the data.
     */
    gem5::Tick read(gem5::PacketPtr pkt) override;

    /**
     * Handle a write to the device
     *
     * @param pkt The memory request.
     * @param data the data
     */
    gem5::Tick write(gem5::PacketPtr pkt) override;
};

} // namespace ArmPerformance

#endif // __ARM_PERFORMANCE_DWT_HH__