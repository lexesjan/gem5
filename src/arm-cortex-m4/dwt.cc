#include "arm-cortex-m4/dwt.hh"

#include "base/trace.hh"
#include "debug/ArmCortexM4DWT.hh"
#include "mem/packet_access.hh"

namespace ArmCortexM4
{

DWT::DWT(const gem5::ArmDWTParams &params)
    : gem5::BasicPioDevice(params, 0x2000),
      control(0x40000001),
      counters(DWT_NCNTRS)
{
}

DWT::~DWT() {}

void
DWT::incrementCounterValue(uint32_t registerAddr, uint32_t value)
{
    assert(isValidCounterRegisterAddr(registerAddr));

    DPRINTF(ArmCortexM4DWT, "Incrementing counter %#x by value %d\n",
        registerAddr, value);
    uint32_t id = DWT::registerAddrToId(registerAddr);
    counters[id] += value;
}

gem5::Tick
DWT::read(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr registerAddr = pkt->getAddr() - pioAddr;

    DPRINTF(ArmCortexM4DWT, "Reading from DWT at offset: %#x\n", registerAddr);
    switch(registerAddr) {
        case DWT_CTRL: {
            pkt->setLE<uint32_t>(control);
            break;
        }
        case DWT_CYCCNT ... DWT_LSUCNT: {
            uint32_t id = DWT::registerAddrToId(registerAddr);
            pkt->setLE<uint32_t>(counters[id]);
            break;
        }
        case DWT_PCSR ... DWT_FUNCTION3:
        case PID4 ... CID3: {
            warn("Unimplemented DWT register read: %#x\n", registerAddr);
            pkt->setLE<uint32_t>(0);
            break;
        }
        default: {
            panic("Unexpected DWT register read: %#x\n", registerAddr);
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

    gem5::Addr registerAddr = pkt->getAddr() - pioAddr;

    DPRINTF(ArmCortexM4DWT, "Writing to DWT at offset: %#x\n", registerAddr);
    switch(registerAddr) {
        case DWT_CYCCNT ... DWT_LSUCNT: {
            uint32_t id = DWT::registerAddrToId(registerAddr);
            uint32_t data = pkt->getLE<uint32_t>();
            counters[id] = data;
            break;
        }
        case DWT_CTRL:
        case DWT_PCSR ... DWT_FUNCTION3:
        case PID4 ... CID3: {
            warn("Unimplemented DWT register write: %#x\n", registerAddr);
            pkt->setLE<uint32_t>(0);
            break;
        }
        default: {
            panic("Unexpected DWT register write: %#x\n", registerAddr);
        }
    }

    pkt->makeAtomicResponse();

    return pioDelay;
}

}  // namespace ArmCortexM4