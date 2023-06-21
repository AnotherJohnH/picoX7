#------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
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

class CxxFile:
   def __init__(self, filename, description):

      self.start_of_line = True
      self.indent        = ''
      self.file          = open(filename, 'w')

      self.header(description)

      if filename[-2:] == '.h':
         self.p('#pragma once')
         self.p()

   def __del__(self):
      self.file.close()

   def push(self, text = '', end = '\n'):
      if text != '':
         self.p(text, end)
      self.indent += ' '*3

   def pop(self, text = '', end = '\n'):
      self.indent = self.indent[:-3]
      if text != '':
         self.p(text, end)

   def p(self, text = '', end = '\n'):

      text += end
      if self.start_of_line:
         text = self.indent + text

      self.file.write(text)

      self.start_of_line = text[-1] == '\n'

   def header(self, description, year = 2022, author = 'John D. Haughton'):
      self.p('//------------------------------------------------------------------------------')
      self.p(f'// Copyright (c) {year} {author}')
      self.p('//')
      self.p('// Permission is hereby granted, free of charge, to any person obtaining a copy')
      self.p('// of this software and associated documentation files (the "Software"), to deal')
      self.p('// in the Software without restriction, including without limitation the rights')
      self.p('// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell')
      self.p('// copies of the Software, and to permit persons to whom the Software is')
      self.p('// furnished to do so, subject to the following conditions:')
      self.p('//')
      self.p('// The above copyright notice and this permission notice shall be included in')
      self.p('// all copies or substantial portions of the Software.')
      self.p('//')
      self.p('// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR')
      self.p('// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,')
      self.p('// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE')
      self.p('// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER')
      self.p('// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,')
      self.p('// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE')
      self.p('// SOFTWARE.')
      self.p('//------------------------------------------------------------------------------')
      self.p('//')
      self.p(f'// \\brief {description}')
      self.p('//')
      self.p('// AUTO-GENERATED')
      self.p('//')
      self.p()
