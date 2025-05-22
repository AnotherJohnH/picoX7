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

#if not defined(HW_NATIVE)

#include "MTL/MTL.h"
#include "MTL/chip/Clocks.h"
#include "MTL/chip/Sio.h"

#endif

#include "DX7/DX7Synth.h"
#include "hw/hw.h"

#include "Usage.h"

// ------------------------------------------------------------------------------------

#if not defined(HW_NATIVE)

//! Select a system clock with clean division to 49.1 KHz
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_137_48_MHZ; }

//! Select a system clock with clean division to 49.095 KHz
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_157_10_MHZ; }

//! Select a system clock with clean division to 49.096 KHz
namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_191_08_MHZ; }

#endif

static const unsigned DAC_FREQ         = 49096;                 //!< DAC sample rate (Hz)
static const unsigned TICK_RATE        = 375;                   //!< 6800 firmware tick (375 Hz)
static const unsigned SAMPLES_PER_TICK = DAC_FREQ / TICK_RATE;  //!< DAC buffer size (16 bit samples)
static const unsigned BUFFER_SIZE      = SAMPLES_PER_TICK / 2;  //!< DAC buffer size (32 bit sample pairs)
static const unsigned NUM_VOICES       = 8;                     //!< Polyphony
static const bool     PROFILE0         = false;                 //!< Resource profiling (core0)
static const bool     PROFILE1         = false;                 //!< Resource profiling (core1)
static const bool     MIDI_DEBUG       = false;

static DX7::Synth<NUM_VOICES, /* AMP_N */ 4> synth {};
static Usage                                 usage {};
static hw::MidiIn                            midi_in {synth, MIDI_DEBUG};
static hw::Led                               led {};


// --- USB MIDI ----------------------------------------------------------------

#if defined(HW_MIDI_USB_DEVICE)

static hw::MidiUSBDevice midi_usb {synth, 0x91C0, "picoX7", MIDI_DEBUG};

extern "C" void IRQ_USBCTRL() { midi_usb.irq(); }

#endif


// --- 7-segment LED display ---------------------------------------------------

#if defined(HW_LED_7_SEG)

static hw::Led7Seg led_7seg;

#endif

void SynthIO::displayLED(unsigned number)
{
#if defined(HW_LED_7_SEG)
   led_7seg.printDec(number, number >= 100 ? 0 : 3);
#endif
}


// --- 16x2 LCD display --------------------------------------------------------

#if not defined(HW_LCD_NONE)

static hw::Lcd lcd{};

#endif

void SynthIO::displayLCD(unsigned row, const char* text)
{
#if not defined(HW_LCD_NONE)
   if (PROFILE0 || PROFILE1)
   {
      static char temp[32];

      switch(row)
      {
      case 0: snprintf(temp, sizeof(temp), "FLASH: %2u%%", usage.getFLASHUsage()); break;
      case 1: snprintf(temp, sizeof(temp), "RAM:   %2u%%", usage.getRAMUsage());   break;
      }

      text = temp;
   }

   lcd.move(0, row);
   lcd.print(text);
#endif
}


// --- VARIABLE CONTROL --------------------------------------------------------

unsigned SynthIO::readControl()
{
#if defined(HW_ADC)
   static bool    first_pass{true};
   static hw::Adc adc{};

   if (first_pass)
   {
      adc.startCont(/* channel */ 0);
   }

   return adc.scaledResult(100);
#else
   return 0;
#endif
}


// --- BUTTON MATRIX -----------------------------------------------------------

uint8_t SynthIO::readButtons(uint8_t row_)
{
   return 0;
}


// --- DAC ---------------------------------------------------------------------

static hw::Audio<SAMPLES_PER_TICK> audio{DAC_FREQ};

#if defined(HW_DAC_I2S)

MTL_AUDIO_ATTACH_IRQ_0(audio);

static MTL::Sio sio;

//! DAC pump call-back
void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   if (PROFILE0)
      usage.start();

   // Wakeup core-1 with
   sio.txFifoPush(uint32_t(buffer));
   __asm__("sev");

   for(unsigned i = 0; i < SAMPLES_PER_TICK; i += 2)
   {
      buffer[i + 1] = synth.getSamplePair(0, NUM_VOICES / 2);
   }

   synth.tick(0, NUM_VOICES / 2);

   if (PROFILE0)
      usage.end();
}

void main_1()
{
   MTL::SysTick sys_tick;

   while(true)
   {
      // Wait for wakeup from core-0
      while(sio.rxFifoEmpty())
      {
         __asm__("wfe");
      }

      if (PROFILE1)
         usage.start();

      uint32_t* buffer = (uint32_t*)sio.rxFifoPop();

      for(unsigned i = 0; i < SAMPLES_PER_TICK; i += 2)
      {
         buffer[i + 0] = synth.getSamplePair(NUM_VOICES / 2, NUM_VOICES);
      }

      synth.tick(NUM_VOICES / 2, NUM_VOICES);

      if (PROFILE1)
         usage.end();
   }
}

#elif defined(HW_DAC_PWM)

MTL_AUDIO_ATTACH_IRQ_0(audio);

static MTL::Sio sio;

//! DAC pump call-back
void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   if (PROFILE0)
      usage.start();

   // Wakeup core-1 with
   sio.txFifoPush(uint32_t(buffer));
   __asm__("sev");

   uint16_t* left_buffer = (uint16_t*)buffer;

   for(unsigned i = 0; i < SAMPLES_PER_TICK; i++)
   {
      int16_t sample = synth.getSample(0, NUM_VOICES / 2);
      left_buffer[i * 2] = audio.packSamples(sample, 0);
   }

   synth.tick(0, NUM_VOICES / 2);

   if (PROFILE0)
      usage.end();
}

void main_1()
{
   MTL::SysTick sys_tick;

   while(true)
   {
      // Wait for wakeup from core-0
      while(sio.rxFifoEmpty())
      {
         __asm__("wfe");
      }

      if (PROFILE1)
         usage.start();

      uint16_t* right_buffer = ((uint16_t*)sio.rxFifoPop()) + 1;

      for(unsigned i = 0; i < SAMPLES_PER_TICK; i++)
      {
         int16_t sample = synth.getSample(NUM_VOICES / 2, NUM_VOICES);
         right_buffer[i * 2] = audio.packSamples(sample, 0);
      }

      synth.tick(NUM_VOICES / 2, NUM_VOICES);

      if (PROFILE1)
         usage.end();
   }
}

#elif defined(HW_DAC_NATIVE)

template<>
void hw::Audio<SAMPLES_PER_TICK>::getSamples(int16_t* buffer, unsigned n)
{
   (void) BUFFER_SIZE;

   for(unsigned i = 0; i < n; i += 2)
   {
      buffer[i + 1] = buffer[i] = synth.getSample(0, NUM_VOICES);
   }

   synth.tick(0, NUM_VOICES);
}

#endif


// --- Entry point -------------------------------------------------------------

int main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : picoX7 (%s)\n", HW_DESCR);
   printf("Author   : Copyright (c) 2023 John D. Haughton\n");
   printf("License  : MIT\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

#if defined(HW_DAC_I2S) || defined(HW_DAC_PWM)
   MTL_start_core(1, main_1);
#endif

   synth.start();

   synth.programChange(0, 0);

   audio.start();

   while(true)
   {
      midi_in.tick();

#if defined(HW_MIDI_USB_DEVICE)
      midi_usb.tick();
#endif

      led = synth.isAnyVoiceOn();

      if (PROFILE0 || PROFILE1)
      {
#if defined(HW_LED_7_SEG)
         led_7seg.printDec(usage.getCPUUsage(), 3);
         usleep(100000);
#else
         (void) usage;
#endif
      }
   }

   return 0;
}
