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

   parser.add_argument('-t', '--track', dest='track', type=int, default=2,
                       help='Track to play [%(default)s]', metavar='<track>')

   parser.add_argument(dest='file', type=str, default=None,
                       help='MIDI file', metavar='<midi-file>')

   return parser.parse_args()

#--------------------------------------------------------------------------------

args  = parseArgs()
file  = MIDI.File(args.file)
midi  = MIDI.Out(args.midi_out)
timer = Timer.Timer(0.005)

for delta_t, command in file.getTrack(args.track):

   timer.join(delta_t)
   midi.send(command)
