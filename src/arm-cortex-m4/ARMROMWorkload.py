# Adapted from https://github.com/devplayer0/gem5/blob/perfgrade/src/perfgrade/ARMROMWorkload.py

from m5.params import *
from m5.objects.Workload import StubWorkload

class ARMROMWorkload(StubWorkload):
    type = 'ARMROMWorkload'
    cxx_class = 'ARMCortexM4::ARMROMWorkload'
    cxx_header = 'arm-cortex-m4/workload.hh'

    rom_file = Param.String('', 'File that contains the ROM')
