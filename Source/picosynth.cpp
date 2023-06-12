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

// \brief test PIO Audio driver

#include <cstdio>

#include "MTL/MTL.h"
#include "MTL/Pins.h"
#include "MTL/PioAudio.h"

#include "Usage.h"
#include "Table_sine.h"

//! 48 KHz, with pinout for Waveshare Pico-Audio
MTL::PioAudio<MTL::Pio0> audio{48000, MTL::PIN_31, MTL::PIN_29, MTL::PIN_32};

PIO_AUDIO_ATTACH_IRQ_0(audio);

Usage usage {};

unsigned phase = 0;

//! 
void MTL::PioAudio_getSamples(uint32_t* buffer, unsigned n)
{
   usage.start();

   for(unsigned i = 0; i < n; ++i)
   {
      uint16_t sample = table_sine[phase] >> 2;

      buffer[i] = (sample << 16) | sample;

      phase = (phase + 3) & TABLE_SINE_MASK;
   }

   usage.end();
}


int MTL_main()
{
   audio.start();

   while(true)
   {
      puts("\033[H");

      printf("FLASH: %2u%%   ", usage.getFLASHUsage());
      printf("RAM: %2u%%   ", usage.getRAMUsage());
      printf("CPU: %2u%%\n", usage.getCPUUsage());

      usage.wait(40000);
   }

   return 0;
}
