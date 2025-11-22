#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

def dx_exp_32(index_14, x):
   """ 14-bit (Q4.10) => 22-bit 2^x table for OPS to calculate phase increment (freq) """
   exp = (index_14 % 1024) / 1024

# NOTE: The more obvious formula is int(math.pow(2.0, i / 1024) * 64 + 0.5). The formula used
#       below more accurately represents the values compressed into the actual ROM as
#       discovered and represented by Ken Shirriff
   exp22 = (int(math.pow(2.0, exp) * 0x800 + 0.5) << (index_14 >> 10)) >> 5

# Re-scale to 32 bits incoporating the re-use of the top 0xC00 table values
# for very low frequencies
   return exp22 << 13 if index_14 < 0x3400 else exp22 >> 3

table.gen('dx_exp_32',
          func      = dx_exp_32,
          typename  = "uint32_t",
          log2_size = 14,
          fmt       = '08x',
          is_const  = False)
