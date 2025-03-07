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

def cartridge(filename, name, offset=6, log2_size=12):
   """ Convert a binary file to a C byte array """

   data = []

   with open(filename, 'rb') as file:
      while True:
         byte = file.read(1)
         if byte == b'':
            break
         data.append(int.from_bytes(byte, byteorder='big', signed=False))

   table.gen(name,
             func      = lambda i,x : data[i + offset],
             typename  = "uint8_t",
             log2_size = log2_size,
             fmt       = '02x')

#------------------------------------------------------------------------------

def dx7_exp_19(index_6, x):
   """ 6-bit (Q4.2) => 19-bit (Q16.3) 2^x table for EGS to calculate level increment (rate) """
   return int(8 * pow(2, index_6 / 4))


def dx7_exp_14(index_14, x):
   """ 14-bit (Q4.10) => 14-bit 2^x table for OPS to calculate linear signal (amplitude) """
   index_14 = 0x3FFF - index_14
   exp      = (index_14 % 1024) / 1024

# NOTE: The more obvious formula is int(math.pow(2.0, i / 1024)/4 + 0.5). The formula used
#       below more accurately represents the values compressed into the actual ROM as
#       discovered and represented by Ken Shirriff
   return (int(math.pow(2.0, exp) * 0x800 + 0.5) << (index_14 >> 10)) >> 13


def dx7_exp_32(index_14, x):
   """ 14-bit (Q4.10) => 22-bit 2^x table for OPS to calculate phase increment (freq) """
   exp = (index_14 % 1024) / 1024

# NOTE: The more obvious formula is int(math.pow(2.0, i / 1024) * 64 + 0.5). The formula used
#       below more accurately represents the values compressed into the actual ROM as
#       discovered and represented by Ken Shirriff
   exp22 = (int(math.pow(2.0, exp) * 0x800 + 0.5) << (index_14 >> 10)) >> 5

# Re-scale to 32 bits incoporating the re-use of the top 0xC00 table values
# for very low frequencies
   return exp22 << 13 if index_14 < 0x3400 else exp22 >> 3


def dx7_log_sine_14(index_12, x):
   """ 12-bit => 14-bit abs-log-sine table for OPS simulation """
   phase = ((index_12 + 0.5) * math.pi) / 2048
   return int(-math.log(abs(math.sin(phase)), 2) * 1024 + 0.5002)


#------------------------------------------------------------------------------

# Maths function lookup tables
table.gen('dx7_exp_19',      func = dx7_exp_19,      typename = "uint32_t", log2_size =  6, fmt = '05x')
table.gen('dx7_exp_14',      func = dx7_exp_14,      typename = "uint16_t", log2_size = 14, fmt = '04x')
table.gen('dx7_exp_32',      func = dx7_exp_32,      typename = "uint32_t", log2_size = 14, fmt = '08x')
table.gen('dx7_log_sine_14', func = dx7_log_sine_14, typename = "uint16_t", log2_size = 12, fmt = '04x')

# Program cartridges
cartridge(sys.argv[1], "dx7_rom_1")
cartridge(sys.argv[2], "dx7_rom_2")
cartridge(sys.argv[3], "dx7_rom_3")
cartridge(sys.argv[4], "dx7_rom_4")
