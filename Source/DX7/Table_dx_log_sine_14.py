#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

def dx_log_sine_14(index_12, x):
   """ 12-bit => 14-bit abs-log-sine table for OPS simulation """
   phase = ((index_12 + 0.5) * math.pi) / 2048
   return int(-math.log(abs(math.sin(phase)), 2) * 1024 + 0.5002)

table.gen('dx_log_sine_14',
          func = dx_log_sine_14,
          typename = "uint16_t",
          log2_size = 12,
          fmt = '04x')
