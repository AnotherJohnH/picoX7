#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

def dx_exp_19(index_6, x):
   """ 6-bit (Q4.2) => 19-bit (Q16.3) 2^x table for EGS to calculate level increment (rate) """
   return int(8 * pow(2, index_6 / 4))

table.gen('dx_exp_19',
          func = dx_exp_19,
          typename = "uint32_t",
          log2_size = 6,
          fmt = '05x')
