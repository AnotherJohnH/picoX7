//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief DX7 simulation for Raspberry Pi Pico

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoX7/Config.h"
#include "Hardware/FilePortal.h"

#if not defined(HW_NATIVE)

#include "MTL/MTL.h"
#include "MTL/chip/Clocks.h"
#include "MTL/chip/Sio.h"

#endif

#include "DX7/DX7Synth.h"


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
static const unsigned NUM_VOICES       = 16;                    //!< Polyphony
static const bool     PROFILE          = false;                 //!< Resource profiling
static const bool     MIDI_DEBUG       = false;

static hw::FilePortal file_portal{"picoX7",
                                  "https://github.com/AnotherJohnH/picoX7/"};

static DX7::Synth<NUM_VOICES, /* AMP_N */ 4> synth{};


// --- Profiler ----------------------------------------------------------------

static hw::Profiler<PROFILE> profiler_core0{};
static hw::Profiler<PROFILE> profiler_core1{};


// --- Physical MIDI -----------------------------------------------------------

static hw::PhysMidi phys_midi{};


// --- USB MIDI ----------------------------------------------------------------

static hw::UsbFileMidi usb{0x91C0, "picoX7", file_portal};

extern "C" void IRQ_USBCTRL() { usb.irq(); }


// --- 7-segment LED display ---------------------------------------------------

static hw::Led7Seg led_7seg;

void SynthIO::displayLED(unsigned number)
{
   led_7seg.printDec(number, number >= 100 ? 0 : 3);
}


// --- 16x2 LCD display --------------------------------------------------------

static hw::Lcd lcd{};

void SynthIO::displayLCD(unsigned row, const char* text)
{
   if (PROFILE)
      return;

   lcd.move(0, row);
   lcd.print(text);
}


// --- LED ---------------------------------------------------------------------

static hw::Led led{};


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

static hw::Audio<SAMPLES_PER_TICK> audio{DAC_FREQ, /* stereo_pairs */ true};

static void hwTick();

#if defined(HW_DAC_I2S)

MTL_AUDIO_ATTACH_IRQ_0(audio);

static MTL::Sio sio;

//! DAC pump call-back
void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   profiler_core0.start();

   // Wakeup core-1 with
   sio.txFifoPush(uint32_t(buffer));
   __asm__("sev");

   for(unsigned i = 0; i < SAMPLES_PER_TICK; i += 2)
   {
      buffer[i + 1] = synth.getSamplePair(0, NUM_VOICES / 2);
   }

   synth.tick(0, NUM_VOICES / 2);

   profiler_core0.stop();
}

void main_1()
{
   while(true)
   {
      // Wait for wakeup from core-0
      while(sio.rxFifoEmpty())
      {
         __asm__("wfe");
      }

      profiler_core1.start();

      uint32_t* buffer = (uint32_t*)sio.rxFifoPop();

      for(unsigned i = 0; i < SAMPLES_PER_TICK; i += 2)
      {
         buffer[i + 0] = synth.getSamplePair(NUM_VOICES / 2, NUM_VOICES);
      }

      synth.tick(NUM_VOICES / 2, NUM_VOICES);

      profiler_core1.stop();
   }
}

#elif defined(HW_DAC_PWM)

MTL_AUDIO_ATTACH_IRQ_0(audio);

static MTL::Sio sio;

//! DAC pump call-back
void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   profiler_core0.start();

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

   profiler_core0.stop();
}

void main_1()
{
   while(true)
   {
      // Wait for wakeup from core-0
      while(sio.rxFifoEmpty())
      {
         __asm__("wfe");
      }

      profiler_core1.start();

      uint16_t* right_buffer = ((uint16_t*)sio.rxFifoPop()) + 1;

      for(unsigned i = 0; i < SAMPLES_PER_TICK; i++)
      {
         int16_t sample = synth.getSample(NUM_VOICES / 2, NUM_VOICES);
         right_buffer[i * 2] = audio.packSamples(sample, 0);
      }

      synth.tick(NUM_VOICES / 2, NUM_VOICES);

      profiler_core1.stop();
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

static void hwTick()
{
   phys_midi.tick();
   usb.tick();
}

void profileReport()
{
   char text[32];

   lcd.move(0, 0);
   lcd.print(profiler_core0.format(text));

   lcd.move(0, 1);
   lcd.print(profiler_core1.format(text));
}

int main()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("\n");
   puts(file_portal.addREADME("picoX7"));
   printf("\n");

#if defined(HW_DAC_I2S) || defined(HW_DAC_PWM)
   MTL_start_core(1, main_1);
#endif

   synth.start();

   synth.programChange(0, 0);

   usb.setDebug(MIDI_DEBUG);
   usb.attachInstrument(1, synth);

   phys_midi.setDebug(MIDI_DEBUG);
   phys_midi.attachInstrument(1, synth);

   // XXX the AKAI MPK mini MIDI controller sends
   //     program changes on MIDI channel 2 #!@*4%
   usb.attachInstrument(2, synth);
   phys_midi.attachInstrument(2, synth);

   audio.start();

   while(true)
   {
      led = synth.isAnyVoiceOn();

      hwTick();

      if (PROFILE)
      {
         profileReport();
         usleep(500000);
      }
   }

   return 0;
}
