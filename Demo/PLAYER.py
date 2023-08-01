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

import argparse
import Timer
import MIDI

#--------------------------------------------------------------------------------

def parseArgs():

   parser = argparse.ArgumentParser(description='picoX7 MIDI file player')

   parser.add_argument('-o', '--out', dest='midi_out', type=str, default="/dev/cu.usbmodem102",
                       help='MIDI out device [%(default)s]', metavar='<device>')

   parser.add_argument('-t', '--track', dest='track', type=int, default=1,
                       help='Track to play [%(default)s]', metavar='<track>')

   parser.add_argument('-p', '--prog', dest='program', type=int, default=0,
                       help='Voice program', metavar='<program>')

   parser.add_argument(dest='file', type=str, default=None,
                       help='MIDI file', metavar='<midi-file>')

   return parser.parse_args()

#--------------------------------------------------------------------------------

def metaEvent(data):
   """ Handle MIDI file meta events """

   event = data[0]
   size  = data[1]
   body  = data[2:]

   if event == MIDI.META_SEQ_NUMBER:
      number = (body[0] << 8 ) | body[1]
      print(f'SEQ NUMBER {number}')

   elif event >= MIDI.META_TEXT and event <= MIDI.META_DEVICE:
      text      = ''.join(chr(_) for _ in body)
      text_type = ["TEXT", "COPYRIGHT", "TRACK", "INSTRUMENT",
                   "LYRIC", "MARKER", "CUE", "?0x08" , "DEVICE"]
      print(f'{text_type[event - 1]:10} "{text}"')

   elif event == MIDI.META_CHAN_PREFIX:
      print(f'CHAN PRFX  {body[0]}')

   elif event == MIDI.META_MIDI_PORT:
      print(f'MIDI PORT  {body[0]}')

   elif event == MIDI.META_END_OF_TRACK:
      print(f'END OF TRACK')

   elif event == MIDI.META_TEMPO:
      period = (body[0] << 16) | (body[1] << 8 ) | body[2]
      print(f'TEMPO      {period} uS/quarter-note')

   elif event == MIDI.META_SMPTE:
      print(f'SMPTE      {body[0]}:{body[1]:02}:{body[2]:02} {body[3]}:{body[4]:02}')

   elif event == MIDI.META_TIME_SIG:
      print(f'TIME SIG   {body[0]}/{body[1]}  {body[2]} clocks/click {body[3]} noted_32nd_notes/beat')

   elif event == MIDI.META_KEY_SIG:
      if body[0] == 0:
         print(f'KEY SIG    C', end='')
      elif body[0] < 0:
         print(f'KEY SIG    {body[0]}b', end='')
      else:
         print(f'KEY SIG    {body[0]}#', end='')
      print(' minor' if body[1] else ' major')

   else:
      print(f'META {event:02x} {body}')

#--------------------------------------------------------------------------------

args = parseArgs()
file = MIDI.File(args.file)
midi = MIDI.Out(args.midi_out)

print()
print(f'Filename : "{file.filename}"')
print(f'Format   : {file.format}')
print(f'Tracks   : {file.num_tracks}')
print(f'Division : {file.division}')
print(f'Tempo    : {file.tempo:.0f} bpm')
print()

if args.program != 0:
   midi.program(args.program - 1)

timer = Timer.Timer(60 / (file.tempo * file.division))

for delta_t, command in file.track[args.track - 1]:

   timer.join(delta_t)

   if command[0] == MIDI.META_EVENT:
      metaEvent(command[1:])

   else:
      cmd = command[0] & 0xF0

      if cmd == MIDI.PROGRAM_CHANGE and args.program != 0:
         print(f"Ignore CH{command[0] & 0xF} PROGRAM {command[1]}")
      else:
         midi.send(command)
