from m5.params import *
from m5.objects.Device import BasicPioDevice

class ArmDWT(BasicPioDevice):
    type = 'ArmDWT'
    cxx_class = 'ArmCortexM4::DWT'
    cxx_header = 'arm-cortex-m4/dwt.hh'
