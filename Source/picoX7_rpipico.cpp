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
#include <unistd.h>

#include "STB/MIDIInterface.h"

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/Pins.h"
#include "MTL/PioAudio.h"
#include "MTL/Led7Seg.h"
#include "MTL/Lcd.h"
#include "MTL/rp2040/Uart.h"
#include "MTL/rp2040/Clocks.h"
#include "MTL/rp2040/Adc.h"

#include "Usage.h"
#include "DX7/Synth.h"

static const unsigned SAMPLE_RATE = 49096;                   // DAC sample rate (Hz)
static const unsigned TICK_RATE   = 375;                     // 6800 firmware tick (375 Hz)
static const unsigned BUFFER_SIZE = SAMPLE_RATE / TICK_RATE; // DAC buffer size (samples)
static const unsigned NUM_VOICES  = 6;                       // Polyphony

//! Select a system clock with clean division to 49.1 KHz
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_137_48_MHZ; }

//! Select a system clock with clean division to 49.095 KHz
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_157_10_MHZ; }

//! Select a system clock with clean division to 49.096 KHz
namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_191_08_MHZ; }


// --- RP2040 module LED --------------------------------------------------------------

MTL::Digital::Out<MTL::PIN_LED1> led;


// --- MIDI-in -----------------------------------------------------------------

//! MIDI in at 31250 baud
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


// --- debug MIDI-in -----------------------------------------------------------

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


// --- 7-segment LED display ---------------------------------------------------

static MTL::Led7Seg</* PIN_CLK */  MTL::PIN_5,
                    /* PIN_DATA */ MTL::PIN_4,
                    /* NUM_DIGITS */ 2> led_7seg;

void SynthIO::displayLED(unsigned number)
{
   led_7seg.printDec(number, number >= 100 ? 0 : 3);
}


// --- 16x12 LCD display -------------------------------------------------------

static MTL::Lcd</* PIN_DATA */   MTL::PIN_9,
                /* PIN_R_S */    MTL::PIN_19,
                /* PIN_ENABLE */ MTL::PIN_20,
                /* COLS */       16,
                /* ROWS */       2,
                /* DL_8BIT */    true> lcd;

void SynthIO::displayLCD(unsigned row, const char* text)
{
   lcd.move(0, row);
   lcd.print(text);
}


// --- ADC ---------------------------------------------------------------------

MTL::Adc adc;

unsigned SynthIO::readSliderADC()
{
   static bool first_pass{true};

   if (first_pass)
   {
      adc.startCont(/* channel */ 0);
   }

   return adc.scaledResult(100);
}


// -----------------------------------------------------------------------------

static Usage                            usage {};
static Synth<NUM_VOICES, /* AMP_N */ 8> synth {};

// --- DAC ---------------------------------------------------------------------

//! 49.1 KHz I2S DAC, with pinout for Waveshare Pico-Audio
//  buffer sized to give a 375 Hz tick
static MTL::PioAudio<MTL::Pio0,BUFFER_SIZE> audio {SAMPLE_RATE,
                                                   MTL::PIN_27,  // MCLK
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


// --- Entry point -------------------------------------------------------------

int MTL_main()
{
   // Starting
   led = true;

   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

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

      usleep(500000);
#endif
   }

   return 0;
}
