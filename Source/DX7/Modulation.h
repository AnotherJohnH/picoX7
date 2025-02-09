//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

// \brief DX7 firmware simulation - modulation sources
//
// Credit to AJXS for his excellent disassembly of the DX7 firmware ROM

#pragma once

#include <cstdint>

#include "SysEx.h"

//! Model of Yamaha DX7 modulation source firmware
class Modulation
{
public:
   enum Source
   {
      MOD_WHEEL = 0,
      FOOT_CONTROL,
      BREATH_CONTROL,
      AFTER_TOUCH,
      NUM_SOURCES
   };

   Modulation() = default;

   //! Total EG-Bias invrted-range (0-FF)
   uint8_t getEGBiasRange() const { return eg_bias_range; }

   //! Total EG-Bias (0-FF)
   uint8_t getEGBias() const { return eg_bias; }

   //! Total amplitude modulation factor (0-FF)
   uint8_t getAmpMod()   const { return amp_mod; }

   //! Total pitch modulation factor (0-FF)
   uint8_t getPitchMod() const { return pitch_mod; }

   //! Program ranges and assigns from SysEx parameter patch
   void load(const SysEx::Param& param_)
   {
      source[MOD_WHEEL]     .load(param_.mod_wheel_range,      param_.mod_wheel_assign);
      source[FOOT_CONTROL]  .load(param_.foot_control_range,   param_.foot_control_assign);
      source[BREATH_CONTROL].load(param_.breath_control_range, param_.breath_control_assign);
      source[AFTER_TOUCH]   .load(param_.after_touch_range,    param_.after_touch_assign);

      calcTotals();
   }

   //! Supply modulation source raw input (0-FF)
   void rawInput(Source index_, uint8_t value_)
   {
      source[index_].input(value_);

      calcTotals();
   }

private:
   void calcTotals()
   {
      unsigned accum_eg_bias_range = 0;
      unsigned accum_eg_bias       = 0;
      unsigned accum_amp_mod       = 0;
      unsigned accum_pitch_mod     = 0;

      for(unsigned i = 0; i < NUM_SOURCES; ++i)
      {
         if (source[i].isEGBiasEnabled())
         {
            accum_eg_bias_range += source[i].getRange();
            accum_eg_bias       += source[i].getInput();
         }

         if (source[i].isAmpEnabled())
            accum_amp_mod += source[i].getInput();

         if (source[i].isPitchEnabled())
            accum_pitch_mod += source[i].getInput();
      }

      eg_bias_range = accum_eg_bias_range > 0xFF ? 0x00 : ~accum_eg_bias_range;
      eg_bias       = accum_eg_bias       > 0xFF ? 0xFF : accum_eg_bias;
      amp_mod       = accum_amp_mod       > 0xFF ? 0xFF : accum_amp_mod;
      pitch_mod     = accum_pitch_mod     > 0xFF ? 0xFF : accum_pitch_mod;
   }

   //! State for one modulation source
   struct SourceState
   { 
   public:
      SourceState() = default;

      //! Get the assign flag states
      bool isPitchEnabled()  const { return pitch; }
      bool isAmpEnabled()    const { return amp; }
      bool isEGBiasEnabled() const { return eg_bias; }

      //! Return quantized range (0-FF)
      uint8_t getRange() const { return quant_range; }

      //! Return scaled input (0-FF)
      uint8_t getInput() const { return scaled_input; }

      //! Program range and assign flags
      void load(uint8_t range_, uint8_t assign_)
      {
         quant_range = (range_ * 660) >> 8;
         pitch       = (assign_ & 0b001) != 0;
         amp         = (assign_ & 0b010) != 0;
         eg_bias     = (assign_ & 0b100) != 0;

         computeScaledInput();
      }

      //! Update raw input
      void input(uint8_t raw_)
      {
         raw_input = raw_;

         computeScaledInput();
      }

   private:
      void computeScaledInput()
      {
         scaled_input = (raw_input * quant_range) >> 8;
      }

      bool    pitch{false};
      bool    amp{false};
      bool    eg_bias{false};
      uint8_t quant_range{0};
      uint8_t raw_input{0};
      uint8_t scaled_input{0};

   } source[NUM_SOURCES];

   uint8_t eg_bias_range{0}; //!< DX7 var @ 0x00BA 0xFF limited and inverted sum EG Bias ranges
   uint8_t eg_bias{0};       //!< DX7 var @ 0x00D5
   uint8_t amp_mod{0};       //!< DX7 var @ 0x00D6
   uint8_t pitch_mod{0};
};
