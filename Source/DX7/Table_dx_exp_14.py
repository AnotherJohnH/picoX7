#------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#------------------------------------------------------------------------------

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
