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

#pragma once

#include "EnvGen.h"
#include "Egs_EnvGen.h"

//! Model of Yamaha YM21290 EGS
template <unsigned NUM_OP>
class Egs
{
public:
   Egs() = default;

   //! Re-program voice from SysEx
   void prog(const SysEx::Voice* patch)
   {
      for(unsigned i = 0; i < NUM_OP; i++)
      {
         const SysEx::Op& op = patch->op[5 - i];

         egs[i].prog(op.eg_amp, op.out_level);
      }
   }

   //! Set operator envelope generator rates
   void setEgsOpEgRates(unsigned op_index_, const uint8_t* rates_) { op_eg[op_index_].setRates(rates_); }

   //! Set operator envelope generator target levels
   void setEgsOpEgLevels(unsigned op_index_, const uint8_t* levels_) { op_eg[op_index_].setLevels(levels_); }

   //! Set operator amplitude modulation sensitivity
   void setEgsOpAmpModSens(unsigned op_index_, uint8_t sens_)     { op_amp_mod_sens[op_index_] = sens_; }

   //! Set operator rate scaling
   void setEgsOpRateScaling(unsigned op_index_, uint8_t scaling_) { op_rate_scaling[op_index_] = scaling_; }

   //! Set operator pitch mode, fixed or ratio
   void setEgsOpPitchFixed(unsigned op_index_, bool fixed_)       { op_pitch_fixed[op_index_] = fixed_; }

   //! Set operator pitch value
   void setEgsOpPitch(unsigned op_index_, uint16_t pitch_)        { op_pitch[op_index_] = pitch_; }

   //! Set operator detune
   void setEgsOpDetune(unsigned op_index_, uint8_t detune_)       { op_detune[op_index_] = detune_ - 7; }

   //! Set operator lebel
   void setEgsOpLevel(unsigned op_index_, uint8_t level_)         { op_levels[op_index_] = level_; }

   //! Set voice pitch
   void setEgsVoicePitch(uint16_t pitch_)                      { voice_pitch = pitch_; }

   //! Set amplitude modulation
   void setEgsAmpMod(uint8_t modulation_)                      { amp_mod = modulation_; }

   //! Set pitch modulation
   void setEgsPitchMod(uint8_t modulation_)                    { pitch_mod = modulation_; }

   //! Start of note
   void keyOn()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         egs[i].gateOn();
      }
   }

   //! Release of note
   void keyOff()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         egs[i].gateOff();
      }
   }

   //! Check if all amplitude envelopes are at L4
   //! NOTE: This is not functionality performed by a real DX
   bool isComplete() const
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         if (not egs[i].isComplete())
            return false;
      }

      return true;
   }

protected:
   int32_t getEgsAmp(unsigned index)
   {
      return egs[index]();
   }

private:
   // EGS program state
protected:
   EnvGen   egs[NUM_OP];
private:
   Egs_EnvGen op_eg[NUM_OP];
   uint8_t    op_eg_rates[NUM_OP][4];
   uint8_t    op_eg_levels[NUM_OP][4];
   uint8_t    op_rate_scaling[NUM_OP] = {0};
   uint8_t    op_amp_mod_sens[NUM_OP] = {0};
   bool       op_pitch_fixed[NUM_OP] = {false};
   uint16_t   op_pitch[NUM_OP] = {0};
   int8_t     op_detune[NUM_OP] = {0};

   // EGS dynamic voice state
   uint8_t  op_levels[NUM_OP] {0};
   uint16_t amp_mod {0};
   uint16_t pitch_mod {0};
   uint16_t voice_pitch {0};
};
