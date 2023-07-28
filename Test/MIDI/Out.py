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

class Out:
   """ API for sending MIDI messages out to a MIDI instrument """

   def __init__(self, device):
      self.out     = open(device, "wb")
      self.channel = 0

   def __del__(self):
      if self.out:
         self.out.close()

   def send(self, data):
      self.out.write(bytearray(data))
      self.out.flush()

   def noteOff(self, note, level = 0):
      self.send([0x80 | self.channel, note, level])

   def noteOn(self, note, level = 127):
      self.send([0x90 | self.channel, note, level])

   def noteLevel(self, note, level):
      self.send([0xA0 | self.channel, note, level])

   def control(self, number, value):
      self.send([0xB0 | self.channel, number, value])

   def allSoundsOff(self):
      self.control(0x78, 0)

   def resetAllControllers(self):
      self.control(0x79, 0)

   def localControl(self, on):
      if on:
         self.control(0x7A, 0)
      else:
         self.control(0x7A, 0x7F)

   def allNotesOff(self):
      self.control(0x7B, 0)

   def omniMode(self, on):
      if on:
         self.control(0x7D, 0)
      else:
         self.control(0x7C, 0)

   def monoMode(self, num_channles = 0):
      self.control(0x7E, num_channels)

   def polyMode(self):
      self.control(0x7F, 0)

   def program(self, number):
      self.send([0xC0 | self.channel, number])

   def level(self, level):
      self.send([0xD0 | self.channel, level])

   def pitchBend(self, value):
      self.send([0xE0 | self.channel, value & 0x7F, value >> 7])

   def sysex(self, data):
      self.send([0xF0] + data + [0xF7])