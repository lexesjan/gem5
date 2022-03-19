from m5.params import *
from m5.objects.Device import BasicPioDevice

class ArmInstCounter(BasicPioDevice):
    type = 'ArmInstCounter'
    cxx_class = 'ArmPerformance::InstCounter'
    cxx_header = 'arm_performance/inst_counter.hh'
