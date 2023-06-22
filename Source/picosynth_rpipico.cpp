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

// \brief Pico Synth - for Raspberry Pi Pico

#include <cstdio>

#include "STB/MIDIInterface.h"

#include "Usage.h"
#include "DX7/Synth.h"

#include "MTL/MTL.h"
#include "MTL/Pins.h"
#include "MTL/PioAudio.h"
#include "MTL/rp2040/Uart.h"
#include "MTL/rp2040/Clocks.h"


class MidiIn : public MIDI::Interface
{
public:
   MidiIn(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return uart.empty(); }

   uint8_t rx() { return uart.rx(); }

private:
   MTL::Uart1 uart{31250, 8, MTL::UART::NONE, 1};
};


static Usage    usage {};
static Synth<8> synth {};
static MidiIn   midi_in {synth};

//! Select a system clock with clean division to 49.1 KHz
namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_137_48_MHZ; }

//! 49.1 KHz I2S DAC, with pinout for Waveshare Pico-Audio
//  buffer size to give a 100 Hz tick
static MTL::PioAudio<MTL::Pio0,/* BUFFER_SIZE */ 491> audio {49100,
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
      SYN::Sample sample = synth();

      buffer[i] = (sample << 16) | (sample & 0xFFFF);
   }

   usage.end();
}

int MTL_main()
{
   printf("Pico Synth - DX7\n");

   audio.start();

   while(true)
   {
      midi_in.tick();

#if 0
      puts("\033[H");
      printf("Pico Synth\n");

      printf("FLASH: %2u%%   ", usage.getFLASHUsage());
      printf("RAM: %2u%%   ", usage.getRAMUsage());
      printf("CPU: %2u%%\n", usage.getCPUUsage());

      usage.wait(40000);
#endif
   }

   return 0;
}
