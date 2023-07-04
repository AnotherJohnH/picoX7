#!/usr/bin/env python3

import math
import sys
import Table
import CxxFile

#------------------------------------------------------------------------------

def readSysExFile(filename):
   """ Read a binary file and return raw data """

   data = []

   with open(filename, 'rb') as file:
      while True:
         byte = file.read(1)
         if byte == b'':
            break
         data.append(int.from_bytes(byte, byteorder='big', signed=False))

   return data

#------------------------------------------------------------------------------

# 14-bit [4:10] => 14-bit   2^x table
Table.build('dx7_exp_14',
      bits       = 16,
      func       = lambda i,x : (int(math.pow(2.0, (i & 0x3FF) / 0x400) * 2048 + 0.5) << (i >> 10)) >> 13,
      log2_size  = 14,
      prefix     = '0x',
      fmt        = '04x')

# 14-bit [4:10] => 22-bit   2^x table
Table.build('dx7_exp_22',
      bits       = 32,
      func       = lambda i,x : (int(math.pow(2.0, (i & 0x3FF) / 0x400) * 2048 + 0.5) << (i >> 10)) >> 5,
      log2_size  = 14,
      prefix     = '0x',
      fmt        = '08x')

# 12-bit => 14-bit   sine table
Table.build('dx7_sine_15',
      bits       = 16,
      func       = lambda i,x : int(math.sin((i + 0.5) * 2 * math.pi / 4096) * 0x3FFF + 0.5002),
      log2_size  = 12,
      typename   = "int16_t",
      prefix     = '',
      fmt        = '6d')

# 12-bit => 14-bit   sine table / 3
Table.build('dx7_sine_div3_15',
      bits       = 16,
      func       = lambda i,x : int(math.sin((i + 0.5) * 2 * math.pi / 4096) * 0x3FFF / 3 + 0.5002),
      log2_size  = 12,
      typename   = "int16_t",
      prefix     = '',
      fmt        = '6d')

# 12-bit => 14-bit   sine table / 5
Table.build('dx7_sine_div5_15',
      bits       = 16,
      func       = lambda i,x : int(math.sin((i + 0.5) * 2 * math.pi / 4096) * 0x3FFF / 5 + 0.5002),
      log2_size  = 12,
      typename   = "int16_t",
      prefix     = '',
      fmt        = '6d')

# 0..99 => 30-bit EG level
Table.build('dx7_level_30',
            bits      = 32,
            func      = lambda i,x : int(x * 0x3FFFFFFF),
            size      = 100,
            typename  = "uint32_t",
            prefix    = '0x',
            fmt       = '08x')

# 0..99 => 30-bit EG rate
Table.build('dx7_rate_30',
            bits      = 32,
            func      = lambda i,x : int(0x3FFFFFFF / (180 * pow(3,-i/10) * 49096)),
            size      = 100,
            typename  = "uint32_t",
            prefix    = '0x',
            fmt       = '08x')

# Program ROM
image = readSysExFile(sys.argv[1])

Table.build('dx7_program',
            bits      = 8,
            func      = lambda i,x : image[i + 6],
            log2_size = 12,
            typename  = "uint8_t",
            prefix    = '0x',
            fmt       = '02x')

