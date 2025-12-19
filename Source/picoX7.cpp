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

#include "DX7/Synth.h"

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
static const bool     MIDI_DEBUG       = false;
static const bool     PROFILE          = false;
static const unsigned NUM_SYNTHS       = 1;


static DX7::Synth<NUM_VOICES, /* AMP_N */ 4> dx7{};
static DX7::Synth<NUM_VOICES, /* AMP_N */ 4>* synth{};
static unsigned                              synth_index{0};

// -----------------------------------------------------------------------------

static hw::Profiler<PROFILE> profiler_core0{};
static hw::Profiler<PROFILE> profiler_core1{};
static hw::PhysMidi          phys_midi{};
static hw::Led7Seg           led_7seg;
static hw::Lcd               lcd{};            //!< 16x2 LCD
static hw::Led               led{};
static hw::Buttons           buttons{/* irq */ false};

// --- USB MIDI and FILE -----------------------------------------------------------

static hw::FilePortal file_portal{"picoX7",
                                  "https://github.com/AnotherJohnH/picoX7/"};

static hw::UsbFileMidi usb{0x91C0, "picoX7", file_portal};

extern "C" void IRQ_USBCTRL() { usb.irq(); }


// --- DAC ---------------------------------------------------------------------

static hw::Audio<SAMPLES_PER_TICK> audio{DAC_FREQ, /* stereo_pairs */ true};

static void hwTick();

#if defined(HW_DAC_I2S)

MTL_AUDIO_ATTACH_IRQ_0(audio);

static MTL::Sio sio;

void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   profiler_core0.start();

   // Wakeup core-1 with
   sio.txFifoPush(uint32_t(buffer));
   __asm__("sev");

   for(unsigned i = 0; i < SAMPLES_PER_TICK; i += 2)
   {
      buffer[i + 1] = dx7.getSamplePair(0, NUM_VOICES / 2);
   }

   dx7.tick(0, NUM_VOICES / 2);

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
         buffer[i + 0] = dx7.getSamplePair(NUM_VOICES / 2, NUM_VOICES);
      }

      dx7.tick(NUM_VOICES / 2, NUM_VOICES);

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
      int16_t sample = dx7.getSample(0, NUM_VOICES / 2);
      left_buffer[i * 2] = audio.packSamples(sample, 0);
   }

   dx7.tick(0, NUM_VOICES / 2);

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
         int16_t sample = dx7.getSample(NUM_VOICES / 2, NUM_VOICES);
         right_buffer[i * 2] = audio.packSamples(sample, 0);
      }

      dx7.tick(NUM_VOICES / 2, NUM_VOICES);

      profiler_core1.stop();
   }
}

#elif defined(HW_DAC_NATIVE)

template<>
void hw::Audio<SAMPLES_PER_TICK>::getSamples32(uint32_t* buffer, unsigned n)
{
   (void) BUFFER_SIZE;

   for(unsigned i = 0; i < n; ++i)
   {
      int16_t mono = dx7.getSample(0, NUM_VOICES);

      buffer[i] = (mono << 16) | (mono & 0xFFFF);
   }

   synth->tick(0, NUM_VOICES);
}

#endif

// -----------------------------------------------------------------------------

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

void initSynth()
{
   switch(synth_index)
   {
   case 0: synth = &dx7;      break;
   }

   usb.attachInstrument(1, *synth);
   phys_midi.attachInstrument(1, *synth);

   // XXX the AKAI MPK mini MIDI controller sends
   //     program changes on MIDI channel 2 #!@*4%
   usb.attachInstrument(2, *synth);
   phys_midi.attachInstrument(2, *synth);

   synth->init();

   synth->programChange(0, 0);
}

int main()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("\n");
   puts(file_portal.addREADME("picoX7"));
   printf("\n");

   led_7seg.printDec(88, 0);

   lcd.print("*    picoX7    *\n");
   lcd.print("*  SYNTHESIZER *");

   usleep(1000000);

#if defined(HW_DAC_I2S) || defined(HW_DAC_PWM)
   MTL_start_core(1, main_1);
#endif

   usb.setDebug(MIDI_DEBUG);
   phys_midi.setDebug(MIDI_DEBUG);

   initSynth();

   audio.start();

   while(true)
   {
      led = synth->isAnyVoiceOn();

      hwTick();

      if (PROFILE)
         profileReport();
      else
      {
         for(unsigned line = 0; line < 2; ++line)
         {
            const char* text = synth->getText(line);
            if (text != nullptr)
            {
               lcd.move(0, line);
               lcd.print(text);
            }
         }
      }

      unsigned number{};
      if (synth->getNumber(number))
      {
         led_7seg.printDec(number, number >= 100 ? 0 : 3);
      }

      bool     down{};
      unsigned index{};

      if (buttons.popEvent(index, down))
      {
         if (down)
         {
            synth_index = (synth_index + 1) % NUM_SYNTHS;

            initSynth();
         }
      }

      usleep(100000);
   }

   return 0;
}
