//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// \brief Audio out interface

#pragma once

#include "hw/Config.h"

#if defined(HW_DAC_WAVESHARE)
#define HW_DAC_I2S

#elif defined(HW_DAC_PIMORONI_VGA_DEMO)
#define HW_DAC_I2S

#elif defined(HW_DAC_PIMORONI_PICO_AUDIO)
#define HW_DAC_I2S

#elif defined(HW_DAC_PWM)
#include "MTL/chip/PwmAudio.h"

#elif defined(HW_DAC_NATIVE)
#include "PLT/Audio.h"

#else
#error "DAC config not set"
#endif

#if defined(HW_DAC_I2S)
#include "MTL/chip/PioAudio.h"
#endif

namespace hw {

#if defined(HW_DAC_WAVESHARE)

#if defined(HW_ADC)

//! I2S DAC, with pinout for Waveshare Pico-Audio (Rev 2.1) adjusted to allow use of ADC0
//! XXX not piggy-back
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  MTL::PIN_29,  // SD
                                                  MTL::PIN_32,  // LRCLK + SCLK
                                                  MTL::PIN_27,  // MCLK
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  true}         // LSB LRCLK / MSB SCLK
   {
   }
};

#else

//! I2S DAC, with pinout for Waveshare Pico-Audio (Rev 2.1)
//! XXX piggy-back
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  MTL::PIN_29,  // SD
                                                  MTL::PIN_32,  // LRCLK + SCLK
                                                  MTL::PIN_31,  // MCLK
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  true}         // LSB LRCLK / MSB SCLK
   {
   }
};

#endif

#elif defined(HW_DAC_PIMORONI_VGA_DEMO)

//! I2S DAC, with pinout for PiMoroni VGA Demo board
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  MTL::PIN_31,     // SD
                                                  MTL::PIN_32,     // LRCLK + SCLK
                                                  MTL::PIN_IGNORE, // No MCLK
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  false}           // LSB LRCLK / MSB SCLK
   {
   }
};

#elif defined(HW_DAC_PIMORONI_PICO_AUDIO)

//! I2S DAC, with pinout for PiMoroni Pico Audio
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  MTL::PIN_12,     // SD
                                                  MTL::PIN_14,     // LRCLK + SCLK
                                                  MTL::PIN_IGNORE, // No MCLK
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  false}           // LSB LRCLK / MSB SCLK
   {
   }
};

#elif defined(HW_DAC_PWM)

template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PwmAudio<MTL::PIN_21, /* BITS */ 8, SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PwmAudio<MTL::PIN_21, /* BITS */ 8, SAMPLES_PER_TICK>{dac_freq}
   {
   }
};

#elif defined(HW_DAC_NATIVE)

template <unsigned SAMPLES_PER_TICK>
class Audio : public PLT::Audio::Out
{
public:
   // XXX requested DAC frequency ignored
   Audio(unsigned dac_freq)
      : PLT::Audio::Out(/* sample_rate */ 48000,
                        PLT::Audio::Format::SINT16,
                        /* channels */ 2,
                        /* samples */ SAMPLES_PER_TICK)
   {}

private:
   void getSamples(int16_t* buffer, unsigned n) override;
};

#endif

} // namespace hw
