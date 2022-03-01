from m5.params import *
from m5.objects.Device import BasicPioDevice

class ARMDWT(BasicPioDevice):
    type = 'ARMDWT'
    cxx_class = 'ARMCortexM4::DWT'
    cxx_header = 'arm-cortex-m4/dwt.hh'
