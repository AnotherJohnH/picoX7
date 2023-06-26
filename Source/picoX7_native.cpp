//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// \brief picoX7 native build for testing

#include <cstdio>

#include "STB/MIDIInterface.h"

#include "DX7/Synth.h"

#include "PLT/Audio.h"
#include "PLT/Event.h"


class MidiIn : public MIDI::Interface
{
public:
   MidiIn(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return n == sizeof(data); }

   uint8_t rx() override { return data[n++]; }

private:
   unsigned n {0};
   uint8_t  data[3] =
   {
      0x90, 0x45, 0x7F
//, 0x44, 0x7F, 0x47, 0x7F
   };
};


static Synth<8> synth {};
static MidiIn   midi_in {synth};

class Monitor : public PLT::Audio::Out
{
public:
   Monitor()
      : PLT::Audio::Out(SYN::SAMPLE_FREQ, PLT::Audio::Format::SINT16, /* channels */ 2)
   {}

private:
   void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; i += 2)
      {
         buffer[i + 1] = buffer[i] = synth();
      }
   }
};


int main()
{
   printf("picoX7\n");

   Monitor monitor;

   monitor.setEnable(true);

   midi_in.tick();

   return PLT::Event::mainLoop();
}
