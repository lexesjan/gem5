# Adapted from https://github.com/devplayer0/gem5/blob/perfgrade/src/perfgrade/ARMROMWorkload.py

from m5.params import *
from m5.objects.Workload import StubWorkload

class ArmROMWorkload(StubWorkload):
    type = 'ArmROMWorkload'
    cxx_class = 'ArmCortexM4::ArmROMWorkload'
    cxx_header = 'arm-cortex-m4/workload.hh'

    rom_file = Param.String('', 'File that contains the ROM')
