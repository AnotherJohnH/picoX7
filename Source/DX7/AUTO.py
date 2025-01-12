#!/usr/bin/env python3
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
import sys
import table

#------------------------------------------------------------------------------

def cartridge(filename, name):
   """ Convert a binary file to a C byte array """

   data = []

   with open(filename, 'rb') as file:
      while True:
         byte = file.read(1)
         if byte == b'':
            break
         data.append(int.from_bytes(byte, byteorder='big', signed=False))

   table.gen(name,
             func      = lambda i,x : data[i + 6],
             typename  = "uint8_t",
             log2_size = 12,
             fmt       = '02x')

#------------------------------------------------------------------------------

# 14-bit (Q4.10) => 14-bit 2^x table
# NOTE: The more obvious formula is int(math.pow(2.0, i / 1024)/4 + 0.5). The formula used
#       below more accurately represents the values compressed into the actual ROMs as
#       discovered and represented by Ken Shirriff
table.gen('dx7_exp_14',
          func       = lambda i,x : (int(math.pow(2.0, (i % 1024) / 1024) * 2048 + 0.5) << (i >> 10)) >> 13,
          typename   = "uint16_t",
          log2_size  = 14,
          fmt        = '04x')

# 14-bit (Q4.10) => 22-bit 2^x table
# NOTE: The more obvious formula is int(math.pow(2.0, i / 1024) * 64 + 0.5). The formula used
#       below more accurately represents the values compressed into the actual ROMs as
#       discovered and represented by Ken Shirriff
table.gen('dx7_exp_22',
          func       = lambda i,x : (int(math.pow(2.0, (i % 1024) / 1024 ) * 2048 + 0.5) << (i >> 10)) >> 5,
          typename   = "uint32_t",
          log2_size  = 14,
          fmt        = '08x')

# 11-bit => 14-bit  half log-sine table
table.gen('dx7_log_sine_14',
          func       = lambda i,x : int(-math.log(math.sin((i + 0.5) * math.pi / 2048), 2) * 1024 + 0.5002),
          typename   = "uint16_t",
          log2_size  = 11,
          fmt        = '04x')

# 12-bit => 14-bit   sine table
table.gen('dx7_sine_15',
          func       = lambda i,x : int(math.sin((i + 0.5) * 2 * math.pi / 4096) * 0x3FFF + 0.5002),
          typename   = "int16_t",
          log2_size  = 12,
          fmt        = '6d')

# 12-bit => 14-bit   sine table / 3
table.gen('dx7_sine_div3_15',
          func       = lambda i,x : int(math.sin((i + 0.5) * 2 * math.pi / 4096) * 0x3FFF / 3 + 0.5002),
          typename   = "int16_t",
          log2_size  = 12,
          fmt        = '6d')

# 12-bit => 14-bit   sine table / 5
table.gen('dx7_sine_div5_15',
          func       = lambda i,x : int(math.sin((i + 0.5) * 2 * math.pi / 4096) * 0x3FFF / 5 + 0.5002),
          typename   = "int16_t",
          log2_size  = 12,
          fmt        = '6d')

# 0..63 => 30-bit EG level
table.gen('dx7_level_30',
           func      = lambda i,x : int(x * (pow(2, 30) - 1)),
           typename  = "uint32_t",
           log2_size = 6,
           fmt       = '08x')

# 0..63 => 30-bit EG rate
table.gen('dx7_rate_30',
           func      = lambda i,x : int(pow(2, 7 + (i/4))),
           typename  = "uint32_t",
           log2_size = 6,
           fmt       = '08x')

# Program cartridges
cartridge(sys.argv[1], "dx7_rom_1")
cartridge(sys.argv[2], "dx7_rom_2")
cartridge(sys.argv[3], "dx7_rom_3")
cartridge(sys.argv[4], "dx7_rom_4")
