#include "arm-cortex-m4/dwt.hh"

#include "base/trace.hh"
#include "debug/ArmCortexM4DWT.hh"
#include "mem/packet_access.hh"

namespace ArmCortexM4
{

DWT::DWT(const gem5::ArmDWTParams &params)
    : gem5::BasicPioDevice(params, 0x2000)
{
}

DWT::~DWT() {}

gem5::Tick
DWT::read(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr daddr = pkt->getAddr() - pioAddr;

    DPRINTF(ArmCortexM4DWT, "Reading from DWT at offset: %#x\n", daddr);
    pkt->setLE<uint32_t>(0xFAFAFAFA);

    pkt->makeAtomicResponse();

    return pioDelay;
}

gem5::Tick
DWT::write(gem5::PacketPtr pkt)
{
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize);
    assert(pkt->getSize() == 4);

    gem5::Addr daddr = pkt->getAddr() - pioAddr;

    DPRINTF(ArmCortexM4DWT, "Writing to DWT at offset: %#x\n", daddr);
    pkt->makeAtomicResponse();

    return pioDelay;
}

}  // namespace ArmCortexM4