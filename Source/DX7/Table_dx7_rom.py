#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import sys
import table

# extract digit from this script name 'Table_dx7_rom_N.py'
index    = sys.argv[0][-4]
filename = sys.argv[1]

data = []

with open(filename, 'rb') as file:
   while True:
      byte = file.read(1)
      if byte == b'':
         break
      data.append(int.from_bytes(byte, byteorder='big', signed=False))

offset = 6

table.gen("dx7_rom_" + index,
          func      = lambda i,x : data[i + offset],
          typename  = "uint8_t",
          log2_size = 12,
          fmt       = '02x')
