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

def build(name,
          bits,
          func,
          log2_size = None,
          size      = None,
          typename  = None,
          include   = "<cstdint>",
          prefix    = '',
          fmt       = 'd'):

   script      = __file__.split("/")[-1]
   comment     = f'// Auto-generated by {script}\n'
   header      = 'Table_' + name + '.h'
   module      = 'Table_' + name + '.cpp'
   symbol      = 'table_' + name
   symbol_size = 'TABLE_' + name.upper() + '_SIZE'
   symbol_mask = 'TABLE_' + name.upper() + '_MASK'

   if log2_size:
      size = 1 << log2_size

   if not typename:
      typename = f'uint{bits}_t'

   with open(header, 'w') as f:

      f.write(comment)
      f.write('\n')
      f.write('#pragma once\n');
      f.write('\n')
      f.write(f'#include {include}\n');
      f.write('\n')

      if log2_size:
         f.write(f'static const unsigned LOG2_{symbol_size} = {log2_size};\n')
         f.write(f'static const unsigned {symbol_size}      = 1 << LOG2_{symbol_size};\n')
         f.write(f'static const unsigned {symbol_mask}      = {symbol_size} - 1;\n')
      else:
         f.write(f'static const unsigned {symbol_size}      = {size};\n')

      f.write('\n')
      f.write(f'extern const {typename} {symbol}[{symbol_size}];\n')

   with open(module, 'w') as f:

      f.write(comment)
      f.write('\n')
      f.write(f'#include "{header}"\n');
      f.write('\n')
      f.write(f'const {typename} {symbol}[{symbol_size}] =\n')
      f.write('{')

      for i in range(0, size):

         x = i / (size - 1)
         y = func(i, x)

         if i % 8 == 0:
            f.write('\n  ')

         f.write(f' {prefix}{y:>{fmt}},')

      f.write('\n};')
