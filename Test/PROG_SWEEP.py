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
import time
import MIDI

#--------------------------------------------------------------------------------

def timeStart():
   global timer
   timer = time.time()

def timeJoin(period):
   global timer
   timer += period
   while time.time() < timer:
      pass

#--------------------------------------------------------------------------------

def parseArgs():

   parser = argparse.ArgumentParser(description='picoX7 test driver')

   parser.add_argument('-o', '--out', dest='midi_out', type=str, default="/dev/cu.usbmodem102",
                       help='MIDI out device [%(default)s]', metavar='<device>')

   parser.add_argument('-r', '--range', dest='range', type=int, default=128,
                       help='Sweep range [%(default)s]', metavar='<range>')

   parser.add_argument('-p', '--period', dest='period', type=int, default=1,
                       help='Period for sweep step (seconds) [%(default)s]', metavar='<period>')

   parser.add_argument('-n', '--note', dest='note', type=int, default=MIDI.NOTE_C4,
                       help='MIDI note to use [%(default)s]', metavar='<note>')

   return parser.parse_args()

#--------------------------------------------------------------------------------

args = parseArgs()

midi = MIDI.Out(args.midi_out)

timeStart()

for program_number in range(0, args.range):

   print(f"Program {program_number}/{args.range}")
   midi.program(program_number)

   timeJoin(args.period)
   midi.noteOn(args.note)

   timeJoin(args.period)
   midi.noteOff(args.note)

timeJoin(args.period)
midi.allSoundsOff()
