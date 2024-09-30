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

#if defined(HW_NATIVE)

#include "PLT/Audio.h"
#include "PLT/MIDIInterface.h"

#else

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/Pins.h"
#include "MTL/Led7Seg.h"
#include "MTL/chip/PioAudio.h"
#include "MTL/chip/PwmAudio.h"
#include "MTL/chip/Uart.h"
#include "MTL/chip/Clocks.h"
#include "MTL/chip/Sio.h"

#endif

#include "Usage.h"

#include "DX7/Synth.h"


// --- Target Hardware Configuration --------------------------------------------------

#if defined(HW_WAVESHARE_GPIO_LCD)

#define HW_DESCR "WaveShare Rev2.1 GPIO LCD"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_UART1
#define HW_DAC_WAVESHARE
#define HW_LED
#define HW_7_SEG_LED
#define HW_LCD_GPIO
#define HW_ADC_NONE

#elif defined(HW_WAVESHARE_I2C_LCD)

#define HW_DESCR "WaveShare Rev2.1 I2C LCD"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_UART1
#define HW_DAC_WAVESHARE
#define HW_LED
#define HW_7_SEG_LED
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E
#define HW_ADC_NONE

#elif defined(HW_PIMORONI_VGA_DEMO)

#define HW_DESCR "Piromoni VGA Demo"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_FAKE
#define HW_DAC_PIMORONI_VGA_DEMO
#define HW_LED
#define HW_7_SEG_LED
#define HW_LCD_NONE
#define HW_ADC_NONE

#elif defined(HW_PIMORONI_PICO_AUDIO)

#define HW_DESCR "Piromoni pico audio"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_UART1
#define HW_DAC_PIMORONI_PICO_AUDIO
#define HW_LED
#define HW_7_SEG_LED
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E
#define HW_ADC_NONE

#elif defined(HW_PWM_DAC)

#define HW_DESCR "Simple PWM DAC"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_UART1
#define HW_DAC_PWM
#define HW_LED
#define HW_7_SEG_LED
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E
#define HW_ADC_NONE

#elif defined(HW_NATIVE)

#define HW_DESCR "native"

#define HW_MIDI_NATIVE
#define HW_DAC_NATIVE
#define HW_LCD_NONE
#define HW_ADC_NONE

#else

#error "Target hardware not specified"

#endif


// ------------------------------------------------------------------------------------

#if defined(HW_NATIVE)

#define HW_DAC_FREQ  48000  //!< DAC sample rate (Hz)

#else

#define HW_DAC_FREQ  49096 //!< DAC sample rate (Hz)

//! Select a system clock with clean division to 49.1 KHz
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_137_48_MHZ; }

//! Select a system clock with clean division to 49.095 KHz
//namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_157_10_MHZ; }

//! Select a system clock with clean division to 49.096 KHz
namespace MTL { Clocks::SysFreq clocks_sys_freq = Clocks::SYS_FREQ_191_08_MHZ; }

#endif

static const unsigned TICK_RATE        = 375;                     // 6800 firmware tick (375 Hz)
static const unsigned SAMPLES_PER_TICK = HW_DAC_FREQ / TICK_RATE; // DAC buffer size (16 bit samples)
static const unsigned BUFFER_SIZE      = SAMPLES_PER_TICK / 2;    // DAC buffer size (32 bit samples pairs)
static const unsigned NUM_VOICES       = 8;                       // Polyphony
static const bool     PROFILE0         = false;                   // Resource profiling (core0)
static const bool     PROFILE1         = false;                   // Resource profiling (core1)

static DX7::Synth<NUM_VOICES, /* AMP_N */ 4> synth {};
static Usage                                 usage {};


// --- MIDI-IN -----------------------------------------------------------------

#if defined(HW_MIDI_UART1)

// pico pin 7 : IN

//! Physical MIDI at 31250 baud
class MidiPhys : public MIDI::Interface
{
public:
   MidiPhys(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return uart.empty(); }

   uint8_t rx() override { return uart.rx(); }

   void tx(uint8_t byte) { return uart.tx(byte); }

private:
   MTL::Uart1_P6_P7 uart{31250, 8, MTL::UART::NONE, 1};
};

static MidiPhys midi_in {synth};

#elif defined(HW_MIDI_FAKE)

//! Fake MIDI in with hard coded messages
class MidiFake : public MIDI::Interface
{
public:
   MidiFake(MIDI::Instrument& instrument)
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

static MidiFake midi_in {synth};

#elif defined(HW_MIDI_NATIVE)

PLT::MIDI::Interface midi_in {synth};

#endif


// --- USB MIDI ----------------------------------------------------------------

#if defined(HW_MIDI_USB_DEVICE)

// pico micro USB : MIDI in

#include "PicoX7USBDevice.h"

class MidiUSB : public MIDI::Interface
{
public:
   MidiUSB(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return device.empty(); }

   uint8_t rx() override { return device.rx(); }

   PicoX7USBDevice device{};
   MTL::Usb        usb{device};
};

static MidiUSB midi_usb {synth};

extern "C" void IRQ_USBCTRL() { midi_usb.usb.irq(); }

#endif


// --- Key press LED ------------------------------------------------------------------

#if defined(HW_LED)

MTL::Digital::Out<MTL::PIN_LED1> led;

#endif


// --- 7-segment LED display ---------------------------------------------------

#if defined(HW_7_SEG_LED)

// pico pin 4 : DATA
// pico pin 5 : CLOCK

static MTL::Led7Seg</* PIN_CLK */  MTL::PIN_5,
                    /* PIN_DATA */ MTL::PIN_4,
                    /* NUM_DIGITS */ 2> led_7seg;

#endif

void SynthIO::displayLED(unsigned number)
{
#if defined(HW_7_SEG_LED)
   led_7seg.printDec(number, number >= 100 ? 0 : 3);
#endif
}


// --- 16x12 LCD display -------------------------------------------------------

#if defined(HW_LCD_GPIO)

// pico pin 9-12  : DATA[0:3]
// pico pin 14-17 : DATA[4:7]
// pico pin 19    : RS
// pico pin 20    : E

#include "MTL/AlphaNumLcd_Gpio.h"

static MTL::AlphaNumLcd</* PIN_DATA */   MTL::PIN_9,
                        /* PIN_R_S */    MTL::PIN_19,
                        /* PIN_ENABLE */ MTL::PIN_20,
                        /* COLS */       16,
                        /* ROWS */       2,
                        /* DL_8BIT */    true> lcd;

#elif defined(HW_LCD_I2C)

// pico pin 19    : SDA
// pico pin 20    : SCL

#include "MTL/chip/I2C.h"
#include "MTL/AlphaNumLcd_I2C.h"

static MTL::AlphaNumLcd<MTL::I2C1_P19_P20,
                        /* COLS */ 16,
                        /* ROWS */ 2> lcd { HW_LCD_I2C_ADDR };

#elif defined(HW_LCD_NONE)

#else

#error "LCD config not set"

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


// --- ADC ---------------------------------------------------------------------

#if defined(HW_ADC)

#include "MTL/chip/Adc.h"

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

#elif defined(HW_ADC_NONE)

unsigned SynthIO::readSliderADC() { return 0; }

#else

#error "ADC config not set"

#endif


// --- DAC ---------------------------------------------------------------------

#if defined(HW_DAC_WAVESHARE)

#define HW_DAC_I2S

#if defined(HW_ADC)

//! 49.1 KHz I2S DAC, with pinout for Waveshare Pico-Audio (Rev 2.1) adjusted to allow use of ADC0
//  buffer sized to give a 375 Hz tick XXX not piggy-back
static MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK> audio {HW_DAC_FREQ,
                                                        MTL::PIN_29,  // SD
                                                        MTL::PIN_32,  // LRCLK + SCLK
                                                        MTL::PIN_27,  // MCLK
                                                        MTL::Audio::STEREO_PAIRS_16};

#else

//! 49.1 KHz I2S DAC, with pinout for Waveshare Pico-Audio (Rev 2.1)
//  buffer sized to give a 375 Hz tick XXX piggy-back
static MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK> audio {HW_DAC_FREQ,
                                                        MTL::PIN_29,  // SD
                                                        MTL::PIN_32,  // LRCLK + SCLK
                                                        MTL::PIN_31,  // MCLK
                                                        MTL::Audio::STEREO_PAIRS_16};

#endif

#elif defined(HW_DAC_PIMORONI_VGA_DEMO)

#define HW_DAC_I2S

//! 49.1 KHz I2S DAC, with pinout for PiMoroni VGA DEMO
//  buffer sized to give a 375 Hz tick
static MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK> audio {HW_DAC_FREQ,
                                                        MTL::PIN_31,     // SD
                                                        MTL::PIN_32,     // LRCLK + SCLK
                                                        MTL::PIN_IGNORE, // No MCLK
                                                        MTL::Audio::STEREO_PAIRS_16,
                                                        false};          // LSB LRCLK / MSB SCLK


#elif defined(HW_DAC_PIMORONI_PICO_AUDIO)

#define HW_DAC_I2S

//! 49.1 KHz I2S DAC, with pinout for PiMoroni VGA DEMO
//  buffer sized to give a 375 Hz tick
static MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK> audio {HW_DAC_FREQ,
                                                        MTL::PIN_12,     // SD
                                                        MTL::PIN_14,     // LRCLK + SCLK
                                                        MTL::PIN_IGNORE, // No MCLK
                                                        MTL::Audio::STEREO_PAIRS_16,
                                                        false};          // LSB LRCLK / MSB SCLK

#endif

#if defined(HW_DAC_I2S)

MTL_AUDIO_ATTACH_IRQ_0(audio);

MTL::Sio sio;

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

static MTL::PwmAudio<MTL::PIN_21, /* BITS */ 8, SAMPLES_PER_TICK> audio{HW_DAC_FREQ};

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

class Audio : public PLT::Audio::Out
{
public:
   Audio()
      : PLT::Audio::Out(HW_DAC_FREQ, PLT::Audio::Format::SINT16, /* channels */ 2)
   {}

private:
   void getSamples(int16_t* buffer, unsigned n) override
   {
      (void) BUFFER_SIZE;

      for(unsigned i = 0; i < n; i += 2)
      {
         buffer[i + 1] = buffer[i] = synth.getSample(0, NUM_VOICES);
      }
   }
};

static Audio audio;

#else

#error "DAC config not set"

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

#if defined(HW_LED)
      led = synth.isAnyVoiceOn();
#endif

      if (PROFILE0 || PROFILE1)
      {
#if defined(HW_7_SEG_LED)
         led_7seg.printDec(usage.getCPUUsage(), 3);
         usleep(100000);
#else
         (void) usage;
#endif
      }
   }

   return 0;
}
