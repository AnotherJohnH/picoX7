#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

""" Sine lookup table """

import math
import table

table.gen("sine",
          func = lambda i, x : int(0x7FFF * math.sin(x * math.pi * 2)),
          typename = "int16_t",
          log2_size = 12,
          fmt = 'd')
