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
import Table
import CxxFile

#------------------------------------------------------------------------------

decode = ['1/100/0 1/000/0 1/000/1 0/001/0 1/010/1 5/011/0',
          '1/000/0 1/000/0 1/000/1 5/001/0 1/110/1 0/011/0',
          '1/100/0 1/000/1 0/001/0 1/010/0 1/010/1 5/011/0',
          '1/000/0 1/000/1 0/101/0 1/010/0 1/010/1 5/011/0',
          '1/100/2 0/001/0 1/010/2 0/011/0 1/010/2 5/011/0',
          '1/000/2 0/101/0 1/010/2 0/011/0 1/010/2 5/011/0',
          '1/100/0 0/001/0 2/011/1 0/001/0 1/010/1 5/011/0',
          '1/000/0 5/001/0 2/111/1 0/001/0 1/010/1 0/011/0',
          '1/000/0 0/001/0 2/011/1 5/001/0 1/110/1 0/011/0',
          '0/001/0 2/011/1 5/001/0 1/110/0 1/010/1 0/011/0',
          '0/101/0 2/011/1 0/001/0 1/010/0 1/010/1 5/011/0',
          '0/001/0 0/011/0 2/011/1 5/001/0 1/110/1 0/011/0',
          '0/101/0 0/011/0 2/011/1 0/001/0 1/010/1 5/011/0',
          '0/101/0 2/011/0 1/000/1 0/001/0 1/010/1 5/011/0',
          '0/001/0 2/011/0 1/000/1 5/001/0 1/110/1 0/011/0',
          '1/100/0 0/001/0 1/010/0 0/011/0 2/011/0 5/001/0',
          '1/000/0 0/001/0 1/010/0 5/011/0 2/111/0 0/001/0',
          '1/000/0 1/000/0 5/001/0 0/111/0 2/011/0 0/001/0',
          '1/100/2 4/001/2 0/011/0 1/010/0 1/010/2 5/011/0',
          '0/001/0 2/011/2 5/001/0 1/110/2 4/011/2 0/011/0',
          '1/001/3 3/001/3 5/011/0 1/110/3 4/011/3 0/011/0',
          '1/100/3 4/001/3 4/011/3 0/011/0 1/010/3 5/011/0',
          '1/100/3 4/001/3 0/011/0 1/010/3 0/011/3 5/011/0',
          '1/100/4 4/001/4 4/011/4 0/011/4 0/011/4 5/011/0',
          '1/100/4 4/001/4 0/011/4 0/011/4 0/011/4 5/011/0',
          '0/101/0 2/011/2 0/001/0 1/010/2 0/011/2 5/011/0',
          '0/001/0 2/011/2 5/001/0 1/110/2 0/011/2 0/011/0',
          '5/001/0 1/110/0 1/010/2 0/011/0 1/010/2 0/011/2',
          '1/100/3 0/001/0 1/010/3 0/011/3 0/011/3 5/011/0',
          '5/001/0 1/110/0 1/010/3 0/011/3 0/011/3 0/011/3',
          '1/100/4 0/001/4 0/011/4 0/011/4 0/011/4 5/011/0',
          '0/101/5 0/011/5 0/011/5 0/011/5 0/011/5 5/011/5']

def AlgTempolates():
   
   c = CxxFile.CxxFile('OpsAlg.h',  'Algorithm')
      
   c.p('#include "OpsBase.h"')
   c.p()
   c.p('class OpsAlg : public OpsBase')
   c.p('{')
   c.p('public:')
   c.p('   OpsAlg() = default;')
   c.p()

   alg = 0

   for line in decode:

      alg += 1

      op_list = line.split(' ')

      # Rotate COM 7th char forward by 1 operator
      com = []
      for index in range(0, 6):
         com.append(op_list[index][6])

      for index in range(0, 6):
         if index == 0:
            op_list[index] = op_list[index][:6] + com[5]
         else:
            op_list[index] = op_list[index][:6] + com[index - 1]

      c.p()
      c.push('')
      c.p(f'int32_t alg{alg}()')
      c.push('{')

      for index in range(0, 6):

         op     = op_list[index]
         field  = op.split('/')
         op_idx = 6 - index

         if op_idx == 1:
            c.p('return ', end = '')
         else:
            c.p('(void) ', end = '')
     
         c.p(f'op<{op_idx}, ', end = '')
         c.p(f'/* SEL */ {field[0]}, ', end = '')
         c.p(f'/* A */ {field[1][0]}, ', end = '')
         c.p(f'/* C */ {field[1][1]}, ', end = '')
         c.p(f'/* D */ {field[1][2]}, ', end = '')
         c.p(f'/* COM */ {int(field[2])+1}', end = '')
         c.p(f'>();')

      c.pop('}')
      c.pop()

   c.p('};')

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
