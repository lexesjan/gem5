from m5.params import *
from m5.SimObject import SimObject

class ARMDWT(SimObject):
    type = 'ARMDWT'
    cxx_class = 'ARMCortexM4::DWT'
    cxx_header = 'arm-cortex-m4/dwt.hh'
