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

from MIDI.Const import *

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
      self.send([NOTE_OFF | self.channel, note, level])

   def noteOn(self, note, level = 127):
      self.send([NOTE_ON | self.channel, note, level])

   def notePressure(self, note, level):
      self.send([POLY_KEY_PRESSURE | self.channel, note, level])

   def control(self, number, value):
      self.send([CONTROL_CHANGE| self.channel, number, value])

   def program(self, number):
      self.send([PROGRAM_CHANGE| self.channel, number])

   def pressure(self, level):
      self.send([CHANNEL_PRESSURE | self.channel, level])

   def pitchBend(self, value):
      self.send([PITCH_BEND | self.channel, value >> 7, value & 0x7F])

   def sysex(self, data):
      self.send([SYSEX_START] + data + [SYSEX_END])

   def allSoundsOff(self):
      self.control(CONTROL_ALL_SOUNDS_OFF, 0)

   def resetAllControllers(self):
      self.control(CONTROL_RESET, 0)

   def localControl(self, on):
      if on:
         self.control(CONTROL_LOCAL_CONTROL, 0)
      else:
         self.control(CONTROL_LOCAL_CONTROL, 0x7F)

   def allNotesOff(self):
      self.control(CONTROL_ALL_NOTES_OFF, 0)

   def omniMode(self, on):
      if on:
         self.control(CONTROL_OMNI_MODE_ON, 0)
      else:
         self.control(CONTROL_OMNI_MODE_OFF, 0)

   def monoMode(self, num_channles = 0):
      self.control(CONTROL_MONO_MODE, num_channels)

   def polyMode(self):
      self.control(CONTROL_POLY_MODE, 0)
