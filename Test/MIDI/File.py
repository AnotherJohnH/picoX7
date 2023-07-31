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

class Chunk:
   """ A class representing a MIDI file chunk """

   def __init__(self, file):
      self.ident  = Chunk.readChar4(file)
      self.length = Chunk.readUint32(file)
      self.data   = file.read(self.length)

   def readChar4(file):
      """ Read 4 char string from file stream """
      bytes = file.read(4)
      return bytes.decode('utf-8')

   def readUint32(file):
      """ Read big endian 32 bit value from file stream """
      bytes = file.read(4)
      return int.from_bytes(bytes, byteorder='big', signed=False)

   def isHeader(self):
      """ Check if chunk is a MIDI file header """
      return self.ident == 'MThd'

   def isTrack(self):
      """ Check if chunk is a MIDI file track """
      return self.ident == 'MTrk'

   def uint16(self, offset):
      """ Extract 16-bit value at byte offset into the chunk data """
      return int.from_bytes(self.data[offset:offset + 2], byteorder='big', signed=False)

   def uint8(self, offset):
      """ Extract 8-bit value at byte offset into the chunk data """
      return int(self.data[offset])

#------------------------------------------------------------------------------

class Header(Chunk):
   """ A class representing a MIDI file header chunk """

   def __init__(self, file):
      """ Construct from an open file stream """

      Chunk.__init__(self, file)
      assert self.isHeader() and self.length == 6

      self.format   = self.uint16(0)
      self.ntrks    = self.uint16(2)
      self.division = self.uint16(4)

#------------------------------------------------------------------------------

class Track(Chunk):
   """ A class representing a MIDI file track chunk """

   def __init__(self, file):
      """ Construct from an open file stream """
      Chunk.__init__(self, file)
      assert self.isTrack()

   def nextByte(self):
      """ Helper to implement __iter__ """

      if self.ptr >= self.length:
         raise StopIteration
      byte = self.uint8(self.ptr)
      self.ptr += 1
      return byte

   def nextBytes(self, n):
      """ Helper to implement __iter__ """

      data = []
      for _ in range(0, n):
         data.append(self.nextByte())
      return data

   def backByte(self):
      self.ptr -= 1

   def __iter__(self):
      self.ptr = 0
      return self

   def __next__(self):

      # Read variable length delta T
      delta_t = 0
      while True:
         byte    = self.nextByte()
         delta_t = (delta_t << 7) | byte & 0b01111111
         if (byte & 0b10000000) == 0:
            break

      # Read command
      command = self.nextBytes(1)

      if (command[0] & 0b10000000) != 0:
         self.cmd = command[0] & 0xF0
         adjust = 0
      else:
         adjust = 1

      if self.cmd in [NOTE_OFF, NOTE_ON, POLY_KEY_PRESSURE, CONTROL_CHANGE, PITCH_BEND]:
         command += self.nextBytes(2 - adjust)

      elif self.cmd in [PROGRAM_CHANGE, CHANNEL_PRESSURE]:
         command += self.nextBytes(1 - adjust)

      elif self.cmd == SYSTEM:

         if command[0] == SYSEX_START:
            while True:
               byte = self.nextByte()
               command.append(byte)
               if byte == SYSEX_END:
                  break

         elif command[0] == META_EVENT:
            # Meta event, event size is the next byte after the event type
            command += self.nextBytes(2)
            command += self.nextBytes(command[2])

      return (delta_t, command)

#------------------------------------------------------------------------------

class File:
   """ A class representing a MIDI file """

   def __init__(self, filename):
      """ Construct from a file """

      self.filename   = filename
      self.tempo      = 120
      self.track_list = []

      with open(filename, "rb") as file:
         self.header = Header(file)
         for track in range(0, self.header.ntrks):
            self.track_list.append(Track(file))

      # Scan meta data in the first track
      for delta_t, command in self.track_list[0]:
         if command[0] == META_EVENT:
            event = command[1]
            data  = command[3:]
            if event == META_TEMPO:
               period = (data[0] << 16) | (data[1] << 8 ) | data[2]
               self.tempo = 60 * 1000000 / period

   @property
   def format(self):
      """ File format 0, 1 or 2 """
      return self.header.format

   @property
   def num_tracks(self):
      """ Number of tracks """
      return self.header.ntrks

   @property
   def division(self):
      """ Delta-ticks per quarter-note """
      return self.header.division

   @property
   def track(self):
      """ Return the track list """
      return self.track_list
