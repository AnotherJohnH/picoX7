#!/usr/bin/env python3

import time
import MidiOut

midi = MidiOut.MidiOut("/dev/cu.usbmodem102")

for p in range(0, 127):

   midi.prog(p)
   midi.noteOn(note = 0x3C)
   time.sleep(1)
   midi.noteOff(note = 0x3C)
