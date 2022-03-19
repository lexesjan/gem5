#include "arm_performance/dwt.hh"

#include "base/trace.hh"
#include "debug/ArmDWT.hh"
#include "mem/packet_access.hh"

namespace ArmPerformance
{

DWT::DWT(const gem5::ArmDWTParams &params)
    : gem5::BasicPioDevice(params, 0x1000),
      control(0x40000001),
      counters(DWT_NCNTRS)
{
}

DWT::~DWT() {}

void
DWT::incrementCounterValue(uint32_t register_addr, uint32_t value)
{
    assert(isValidCounterRegisterAddr(register_addr));

    DPRINTF(ArmDWT, "Incrementing counter %#x by value %d\n",
        register_addr, value);
    uint32_t id = DWT::registerAddrToId(register_addr);
    counters[id] += value;
}

gem5::Tick
DWT::read(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr register_addr = pkt->getAddr() - pioAddr;

    DPRINTF(ArmDWT, "Reading from DWT at offset: %#x\n", register_addr);
    switch(register_addr) {
        case DWT_CTRL: {
            pkt->setLE<uint32_t>(control);
            break;
        }
        case DWT_CYCCNT ... DWT_LSUCNT: {
            uint32_t id = DWT::registerAddrToId(register_addr);
            pkt->setLE<uint32_t>(counters[id]);
            break;
        }
        case DWT_PCSR ... DWT_FUNCTION3:
        case PID4 ... CID3: {
            warn("Unimplemented DWT register read: %#x\n", register_addr);
            pkt->setLE<uint32_t>(0);
            break;
        }
        default: {
            panic("Unexpected DWT register read: %#x\n", register_addr);
        }
    }

    pkt->makeAtomicResponse();

    return pioDelay;
}

gem5::Tick
DWT::write(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr register_addr = pkt->getAddr() - pioAddr;

    DPRINTF(ArmDWT, "Writing to DWT at offset: %#x\n", register_addr);
    switch(register_addr) {
        case DWT_CYCCNT ... DWT_LSUCNT: {
            uint32_t id = DWT::registerAddrToId(register_addr);
            uint32_t data = pkt->getLE<uint32_t>();
            counters[id] = data;
            break;
        }
        case DWT_CTRL:
        case DWT_PCSR ... DWT_FUNCTION3:
        case PID4 ... CID3: {
            warn("Unimplemented DWT register write: %#x\n", register_addr);
            pkt->setLE<uint32_t>(0);
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