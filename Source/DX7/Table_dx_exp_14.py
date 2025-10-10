#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

def dx_exp_14(index_14, x):
   """ 14-bit (Q4.10) => 14-bit 2^x table for OPS to calculate linear signal (amplitude) """
   index_14 = 0x3FFF - index_14
   exp      = (index_14 % 1024) / 1024

# NOTE: The more obvious formula is int(math.pow(2.0, i / 1024)/4 + 0.5). The formula used
#       below more accurately represents the values compressed into the actual ROM as
#       discovered and represented by Ken Shirriff
   return (int(math.pow(2.0, exp) * 0x800 + 0.5) << (index_14 >> 10)) >> 13

table.gen('dx_exp_14',
          func = dx_exp_14,
          typename = "uint16_t",
          log2_size = 14,
          fmt = '04x')
