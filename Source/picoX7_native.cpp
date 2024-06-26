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

static const unsigned SAMPLE_FREQ = 48000;                   // DAC sample rate
static const unsigned NUM_VOICES  = 8;                       // Polyphony

//! Fake MIDI in with hard coded messages
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
   uint8_t  data[7] =
   {
      0x90, 0x3C, 0x7F, 0x40, 0x7F, 0x43, 0x7F
   };
};

// --- 7-segment LED display ---------------------------------------------------

void SynthIO::displayLED(unsigned number)
{
   printf("Program #%02u\n", number);
}

// --- 16x12 LCD display -------------------------------------------------------

void SynthIO::displayLCD(unsigned row, const char* text)
{
   printf("%s\n", text);
}

// --- ADC ---------------------------------------------------------------------

unsigned SynthIO::readSliderADC()
{
   return 50;
}

// -----------------------------------------------------------------------------

static DX7::Synth<NUM_VOICES> synth {};

// --- DAC ---------------------------------------------------------------------

class Audio : public PLT::Audio::Out
{
public:
   Audio()
      : PLT::Audio::Out(SAMPLE_FREQ, PLT::Audio::Format::SINT16, /* channels */ 2)
   {}

private:
   void getSamples(int16_t* buffer, unsigned n) override
   {
      synth.tick();

      for(unsigned i = 0; i < n; i += 2)
      {
         buffer[i + 1] = buffer[i] = synth();
      }
   }
};

static Audio audio;

// --- Entry point -------------------------------------------------------------

int main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : picoX7\n");
   printf("Author   : Copyright (c) 2023 John D. Haughton\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   MidiIn midi_in {synth};

   synth.programChange(0, 0);

   audio.setEnable(true);

   midi_in.tick();

   return PLT::Event::mainLoop();
}
