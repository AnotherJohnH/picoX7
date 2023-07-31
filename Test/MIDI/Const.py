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

# Some helpful constants
NOTE_C4 = 60
NOTE_A4 = 69

# Basic commands
NOTE_OFF          = 0x80
NOTE_ON           = 0x90
POLY_KEY_PRESSURE = 0xA0
CONTROL_CHANGE    = 0xB0
PROGRAM_CHANGE    = 0xC0
CHANNEL_PRESSURE  = 0xD0
PITCH_BEND        = 0xE0
SYSTEM            = 0xF0

# Control types
CONTROL_ALL_SOUNDS_OFF = 0x78
CONTROL_RESET          = 0x79
CONTROL_LOCAL_CONTROL  = 0x7A
CONTROL_ALL_NOTES_OFF  = 0x7B
CONTROL_OMNI_MODE_OFF  = 0x7C
CONTROL_OMNI_MODE_ON   = 0x7D
CONTROL_MONO_MODE      = 0x7E
CONTROL_POLY_MODE      = 0x7F

# System commands
SYSEX_START       = 0xF0
SYSEX_END         = 0xF7
META_EVENT        = 0xFF

# Meta event types
META_SEQ_NUMBER   = 0x00
META_TEXT         = 0x01
META_COPYRIGHT    = 0x02
META_TRACK        = 0x03
META_INSTRUMENT   = 0x04
META_LYRIC        = 0x05
META_MARKER       = 0x06
META_CUE          = 0x07
META_DEVICE       = 0x09
META_CHAN_PREFIX  = 0x20
META_MIDI_PORT    = 0x21
META_END_OF_TRACK = 0x2F
META_TEMPO        = 0x51
META_SMPTE        = 0x54
META_TIME_SIG     = 0x58
META_KEY_SIG      = 0x59
