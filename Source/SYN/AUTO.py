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

#------------------------------------------------------------------------------

Table.build('sine',
      bits      = 16,
      func      = lambda i,x : int(math.sin(x * 2 * math.pi) * 0x7FFF),
      log2_size = 9,
      typename  = "SYN::Sample",
      include   = '"SYN/Sample.h"',
      fmt       = '6d')

Table.build('freq',
      bits   = 32,
      func   = lambda i,x : int(440.0 * math.pow(2.0, (i - 70) / 12.0) * 0x10000),
      size   = 130,
      prefix = '0x',
      fmt    = '08x')

Table.build('ampl',
      bits   = 32,
      func   = lambda i,x : int(i * 0xFFFF / 127),
      size   = 128,
      prefix = '0x',
      fmt    = '08x')
