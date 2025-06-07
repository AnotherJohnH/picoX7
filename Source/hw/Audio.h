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

#if defined(HW_DAC_I2S_WAVESHARE_REV2_1)
#define HW_DAC_I2S

#elif defined(HW_DAC_I2S_PIMORONI_VGA_DEMO)
#define HW_DAC_I2S

#elif defined(HW_DAC_I2S_PIMORONI_PICO_AUDIO)
#define HW_DAC_I2S

#elif defined(HW_DAC_I2S_ANY)
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

#if defined(HW_DAC_I2S_WAVESHARE_REV2_1)

//! I2S DAC, with pinout for Waveshare Pico-Audio (Rev 2.1)
//! piggy-back
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  HW_DAC_I2S_SD,
                                                  HW_DAC_I2S_CLKS,
                                                  HW_DAC_I2S_MCLK,
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  /* LSB LRCLK / MSB SCLK */ true}
   {
      MTL::config.gpio(HW_DAC_I2S_SD,       "I2S SD");
      MTL::config.gpio(HW_DAC_I2S_CLKS,     "I2S LRCLK");
      MTL::config.gpio(HW_DAC_I2S_CLKS + 1, "I2S SCLK");
      MTL::config.gpio(HW_DAC_I2S_MCLK,     "I2S MCLK");
   }
};

#elif defined(HW_DAC_I2S_PIMORONI_VGA_DEMO)

//! I2S DAC, with pinout for PiMoroni VGA Demo board
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  HW_DAC_I2S_SD,
                                                  HW_DAC_I2S_CLKS,
                                                  /* MCLK */ MTL::PIN_IGNORE,
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  /* LSB LRCLK / MSB SCLK */ false}
   {
      MTL::config.gpio(HW_DAC_I2S_SD,       "I2S SD");
      MTL::config.gpio(HW_DAC_I2S_CLKS,     "I2S SCLK");
      MTL::config.gpio(HW_DAC_I2S_CLKS + 1, "I2S LRCLK");
   }
};

#elif defined(HW_DAC_I2S_PIMORONI_PICO_AUDIO)

//! I2S DAC, with pinout for PiMoroni Pico Audio (piggy back)
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  HW_DAC_I2S_SD,
                                                  HW_DAC_I2S_CLKS, 
                                                  /* MCLK */ MTL::PIN_IGNORE,
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  /* LSB LRCLK / MSB SCLK */ false}
   {
      MTL::config.gpio(HW_DAC_I2S_SD,       "I2S SD");
      MTL::config.gpio(HW_DAC_I2S_CLKS,     "I2S SCLK");
      MTL::config.gpio(HW_DAC_I2S_CLKS + 1, "I2S LRCLK");
   }
};

#elif defined(HW_DAC_I2S_ANY)

//! I2S DAC, with pinout in common with other picoX21H project
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  HW_DAC_I2S_SD,
                                                  HW_DAC_I2S_CLKS,
                                                  /* MCLK */ MTL::PIN_IGNORE,
                                                  MTL::Audio::STEREO_PAIRS_16,
                                                  /* LSB LRCLK / MSB SCLK */ false}
   {
      MTL::config.gpio(HW_DAC_I2S_SD,       "I2S SD");
      MTL::config.gpio(HW_DAC_I2S_CLKS,     "I2S SCLK");
      MTL::config.gpio(HW_DAC_I2S_CLKS + 1, "I2S LRCLK");
   }
};

#elif defined(HW_DAC_PWM)

template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PwmAudio<HW_DAC_PWM, /* BITS */ 8, SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq)
      : MTL::PwmAudio<HW_DAC_PWM, /* BITS */ 8, SAMPLES_PER_TICK>{dac_freq}
   {
      MTL::config.gpio(HW_DAC_PWM, "PWM (audio)");
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
