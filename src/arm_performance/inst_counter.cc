#include "arm_performance/inst_counter.hh"

#include "base/trace.hh"
#include "debug/ArmInstCounter.hh"
#include "mem/packet_access.hh"

namespace ArmPerformance
{

const char *instCounterNames[] = {
    [InstCounter::IC_CTRL] = NULL,
    // Move instructions.
    [InstCounter::IC_MOV] = "mov",
    // Add instructions.
    [InstCounter::IC_ADD] = "add",
    [InstCounter::IC_ADC] = "adc",
    // Subtract instructions.
    [InstCounter::IC_SUB] = "sub",
    [InstCounter::IC_SBC] = "sbc",
    [InstCounter::IC_RSB] = "rsb",
    // Multiply instructions.
    [InstCounter::IC_MUL] = "mul",
    // Divide instructions.
    [InstCounter::IC_SDIV] = "sdiv",
    [InstCounter::IC_UDIV] = "udiv",
    // Compare instructions.
    [InstCounter::IC_CMP] = "cmps",
    [InstCounter::IC_CMN] = "cmns",
    // Logical instructions.
    [InstCounter::IC_AND] = "and",
    [InstCounter::IC_EOR] = "eor",
    [InstCounter::IC_ORR] = "orr",
    [InstCounter::IC_ORN] = "orn",
    [InstCounter::IC_BIC] = "bic",
    [InstCounter::IC_MVN] = "mvn",
    [InstCounter::IC_TST] = "tst",
    [InstCounter::IC_TEQ] = "teq",
    // Shift instructions.
    [InstCounter::IC_LSL] = "lsl",
    [InstCounter::IC_LSR] = "lsr",
    [InstCounter::IC_ASR] = "asr",
    // Rotate instructions.
    [InstCounter::IC_ROR] = "ror",
    [InstCounter::IC_RRX] = "rrx",
    // Load instructions.
    [InstCounter::IC_LDR] = "ldr",
    [InstCounter::IC_LDRH] = "ldrh",
    [InstCounter::IC_LDRB] = "ldrb",
    [InstCounter::IC_LDRSH] = "ldrsh",
    [InstCounter::IC_LDRSB] = "ldrsb",
    [InstCounter::IC_LDRD] = "ldrd",
    // gem5 LDM variants (e.g. LDMFD) are given the same name.
    [InstCounter::IC_LDM] = "ldm_uop",
    // Store instructions.
    [InstCounter::IC_STR] = "str",
    [InstCounter::IC_STRH] = "strh",
    [InstCounter::IC_STRB] = "strb",
    [InstCounter::IC_STRSH] = "strsh",
    [InstCounter::IC_STRSB] = "strsb",
    [InstCounter::IC_STRD] = "strd",
    // gem5 STM variants (e.g. STMFD) are given the same name.
    [InstCounter::IC_STM] = "stm_uop",
    // Branch instructions.
    [InstCounter::IC_BL] = "bl",
    [InstCounter::IC_BX] = "bx",
    [InstCounter::IC_B] = "b"
};

InstCounter::InstCounter(const gem5::ArmInstCounterParams &params)
    : gem5::BasicPioDevice(params, 0x1000),
      control(0x1)
{
}

InstCounter::~InstCounter() {}

void
InstCounter::incrementInstCount(const std::string &inst_mnemonic)
{
    DPRINTF(ArmInstCounter, "Incrementing '%s' counter\n", inst_mnemonic);
    instCountMap[inst_mnemonic]++;
}

gem5::Tick
InstCounter::read(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr register_addr = pkt->getAddr() - pioAddr;
    // Align to enum values.
    register_addr /= 4;

    DPRINTF(ArmInstCounter, "Reading from InstCounter at offset: %#x\n",
            register_addr * 4);
    switch (register_addr) {
        case IC_CTRL: {
            pkt->setLE<uint32_t>(control);
            break;
        }
        case IC_MOV ... IC_B: {
            const char *counterName = instCounterNames[register_addr];
            int instCount = instCountMap[counterName];
            pkt->setLE<uint32_t>(instCount);
            break;
        }
        default: {
            panic("Unexpected InstCounter register read: %#x\n",
                  register_addr);
        }
    }

    pkt->makeAtomicResponse();

    return pioDelay;
}

gem5::Tick
InstCounter::write(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr register_addr = pkt->getAddr() - pioAddr;
    // Align to enum values.
    register_addr /= 4;

    DPRINTF(ArmInstCounter, "Writing to InstCounter at offset: %#x\n",
            register_addr * 4);
    switch(register_addr) {
        case IC_MOV ... IC_B: {
            const char *counterName = instCounterNames[register_addr];
            uint32_t instCount = pkt->getLE<uint32_t>();
            instCountMap[counterName] = instCount;
            break;
        }
        case IC_CTRL: {
            warn("Unimplemented DWT register write: %#x\n", register_addr);
            break;
        }
        default: {
            panic("Unexpected DWT register write: %#x\n", register_addr);
        }
    }

    pkt->makeAtomicResponse();

    return pioDelay;
}

}  // namespace ArmPerformance