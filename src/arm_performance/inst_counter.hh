#ifndef __ARM_PERFORMANCE_INST_COUNTER_HH__
#define __ARM_PERFORMANCE_INST_COUNTER_HH__

#include <vector>

#include "base/bitunion.hh"
#include "dev/io_device.hh"
#include "params/ArmInstCounter.hh"

namespace ArmPerformance
{


class InstCounter: public gem5::BasicPioDevice
{
  public:
    InstCounter(const gem5::ArmInstCounterParams &params);
    ~InstCounter();

    enum
    {
        IC_CTRL = 0,
        // Move instructions.
        IC_MOV,
        // Add instructions.
        IC_ADD,
        IC_ADC,
        // Subtract instructions.
        IC_SUB,
        IC_SBC,
        IC_RSB,
        // Multiply instructions.
        IC_MUL,
        // Divide instructions.
        IC_SDIV,
        IC_UDIV,
        // Compare instructions.
        IC_CMP,
        IC_CMN,
        // Logical instructions.
        IC_AND,
        IC_EOR,
        IC_ORR,
        IC_ORN,
        IC_BIC,
        IC_MVN,
        IC_TST,
        IC_TEQ,
        // Shift instructions.
        IC_LSL,
        IC_LSR,
        IC_ASR,
        // Rotate instructions.
        IC_ROR,
        IC_RRX,
        // Load instructions.
        IC_LDR,
        IC_LDRH,
        IC_LDRB,
        IC_LDRSH,
        IC_LDRSB,
        IC_LDRD,
        // gem5 LDM variants (e.g. LDMFD) are given the same name.
        IC_LDM,
        // Store instructions.
        IC_STR,
        IC_STRH,
        IC_STRB,
        IC_STRSH,
        IC_STRSB,
        IC_STRD,
        // gem5 STM variants (e.g. STMFD) are given the same name.
        IC_STM,
        // Branch instructions.
        IC_BL,
        IC_BX,
        IC_B // gem5 conditional instructions are given the same name.
    };

    BitUnion32(IC_CTRL_t)
        Bitfield<0> CNTENA;
    EndBitUnion(IC_CTRL_t)

    /**
     * Increments an instruction count
     *
     * @param inst_mnemonic Instruction mnemonic e.g. 'add'.
     */
    void incrementInstCount(const std::string &inst_mnemonic);

  private:
    /** Control register */
    IC_CTRL_t control;

    /** Instruction count mapping */
    std::unordered_map<std::string, uint32_t> instCountMap;

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

#endif // __ARM_PERFORMANCE_INST_COUNTER_HH__