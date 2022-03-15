from m5.params import *
from m5.objects.Device import BasicPioDevice

class ArmDWT(BasicPioDevice):
    type = 'ArmDWT'
    cxx_class = 'ArmPerformance::DWT'
    cxx_header = 'arm_performance/dwt.hh'
