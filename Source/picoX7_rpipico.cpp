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

// \brief DX7 simulation for Raspberry Pi Pico

#include <cstdio>

#include "STB/MIDIInterface.h"

#include "MTL/MTL.h"
#include "MTL/Pins.h"
#include "MTL/PioAudio.h"
#include "MTL/rp2040/Uart.h"
#include "MTL/rp2040/Clocks.h"

#include "Usage.h"
#include "DX7/Synth.h"

static const unsigned SAMPLE_RATE = 49100;                   // DAC sample rate (Hz)
static const unsigned TICK_RATE   = 375;                     // 6800 firmware tick (375 Hz)
static const unsigned BUFFER_SIZE = SAMPLE_RATE / TICK_RATE; // DAC buffer size (samples)
static const unsigned NUM_VOICES  = 4;                       // Polyphony

//! Select a system clock with clean division to 49.1 KHz
namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_137_48_MHZ; }
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_157_10_MHZ; }
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_191_08_MHZ; }


//! Slow MIDI in via the console UART
class MidiIn0 : public MIDI::Interface
{
public:
   MidiIn0(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return MTL_getch_empty(); }

   uint8_t rx() override { return MTL_getch(); }
};

//! Proper MIDI in at 31250 baud
class MidiIn1 : public MIDI::Interface
{
public:
   MidiIn1(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return uart.empty(); }

   uint8_t rx() override { return uart.rx(); }

private:
   MTL::Uart1 uart{31250, 8, MTL::UART::NONE, 1};
};


static Usage             usage {};
static Synth<NUM_VOICES> synth {};

//! 49.1 KHz I2S DAC, with pinout for Waveshare Pico-Audio
//  buffer sized to give a 375 Hz tick
static MTL::PioAudio<MTL::Pio0,BUFFER_SIZE> audio {SAMPLE_RATE,
                                                   MTL::PIN_31,  // MCLK
                                                   MTL::PIN_29,  // SD
                                                   MTL::PIN_32}; // LRCLK + SCLK
PIO_AUDIO_ATTACH_IRQ_0(audio);

//! DAC pump call-back
void MTL::PioAudio_getSamples(uint32_t* buffer, unsigned n)
{
   usage.start();

   synth.tick();

   for(unsigned i = 0; i < n; ++i)
   {
      int16_t sample = synth();

      // Same 16-nit sample to left and right audio streams
      buffer[i] = (sample << 16) | (sample & 0xFFFF);
   }

   usage.end();
}


int MTL_main()
{
   printf("\n");
   printf("Program  : picoX7\n");
   printf("Author   : Copyright (c) 2023 John D. Haughton\n");
   printf("Version  : %s\n", MTL_VERSION);
   printf("Commit   : %s\n", MTL_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   MidiIn0 midi_in0 {synth};
   MidiIn1 midi_in1 {synth};

   synth.programChange(0, 0);

   audio.start();

   while(true)
   {
      midi_in0.tick();
      midi_in1.tick();

#if 0
      puts("\033[H");
      printf("FLASH: %2u%%   ", usage.getFLASHUsage());
      printf("RAM: %2u%%   ", usage.getRAMUsage());
      printf("CPU: %2u%%\n", usage.getCPUUsage());

      usage.wait(40000);
#endif
   }

   return 0;
}
