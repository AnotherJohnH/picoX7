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

#include "OpsAlg.h"
#include "EnvGen.h"

//! Model of Yamaha EGS (like the YM21290)
class Egs : public OpsAlg<EnvGen>
{
public:
   Egs()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         op[i].env_gen = getEgPointer(i);
      }
   }

   // EGS inputs

   //! Set voice pitch [0x3000..301F] lS 2 bits ignored
   void setEgsVoicePitch(uint16_t pitch16_)
   {
      voice_pitch14 = pitch16_ >> 2;

#if 0
      for(unsigned op_index = 0; op_index < 6; ++op_index)
      {
         unsigned rate_scale = voice_pitch14 + op[op_index].rate_scale3;

         // op[op_index].env_gen->setRateScale(rate_scale);
      }
#endif
   }

   //! Set operator pitch value [0x3020..302F]
   void setEgsOpPitch(unsigned op_index_, uint16_t pitch_)
   {
      op[op_index_].pitch_ratio14 = pitch_ >> 2;
   }

   //! Set operator pitch mode, fixed or ratio
   void setEgsOpPitchFixed(unsigned op_index_, bool fixed_)
   {
      op[op_index_].pitch_fixed = fixed_;
   }

   //! Set operator detune [0x3030..303F]
   void setEgsOpDetune(unsigned op_index_, signed detune4_)
   {
      op[op_index_].detune4 = detune4_;
   }

   //! Set an operator envelope generator rate [0x3040..305F]
   void setEgsOpEgRate(unsigned op_index_, unsigned index_, uint8_t rate6_)
   {
      op[op_index_].env_gen->setRate(index_, rate6_);
   }

   //! Set operator envelope generator target attenuation level [0x3060..307F]
   void setEgsOpEgLevel(unsigned op_index_, unsigned index_, const uint8_t atten6_)
   {
      op[op_index_].env_gen->setAtten(index_, atten6_);
   }

   //! Set operator attenuation level [0x3080..]
   void setEgsOpLevel(unsigned op_index_, uint8_t level8_)
   {
      op[op_index_].env_gen->setOpAtten(level8_);
   }

   //! Set operator rate scaling
   void setEgsOpRateScaling(unsigned op_index_, uint8_t scale3_)
   {
      op[op_index_].rate_scale3 = scale3_;
   }

   //! Set operator amplitude modulation sensitivity
   void setEgsOpAmpModSens(unsigned op_index_, uint8_t sens_)
   {
      op[op_index_].env_gen->setAmpModSens(sens_);
   }

   //! Set amplitude modulation (0..FF)
   void setEgsAmpMod(uint8_t amp_mod8_)
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         op[op_index].env_gen->setAmpMod(amp_mod8_);
      }
   }

   //! Start of note
   void keyOn()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         op[op_index].env_gen->keyOn();
      }

      Ops<NUM_OP,EnvGen>::keyOn();
   }

   //! Release of note
   void keyOff()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         op[op_index].env_gen->keyOff();
      }
   }

   //! Set pitch modulation
   void setEgsPitchMod(int16_t pitch_mod_)
   {
      pitch_mod12 = pitch_mod_ >> 4;
   }

   //! Check if all amplitude envelopes are at L4
   //! NOTE: This is not functionality performed by a real DX
   bool isComplete() const
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         if (not op[op_index].env_gen->isComplete())
            return false;
      }

      return true;
   }

   //! Used by unit test
   int32_t getEgsAmp(unsigned op_index_)
   {
      return op[op_index_].env_gen->getAtten12();
   }

   //! Send frequency value from the EGS to the OPS
   void sendEgsFreq()
   {
      for(unsigned op_index = 0; op_index < 6; op_index++)
      {
         setOpsFreq(op_index,
                    op[op_index].computeOpsFreq14(voice_pitch14, pitch_mod12));
      }
   }

private:
   static const unsigned NUM_OP = 6;

   class EgsOpState
   {
   public:
      EgsOpState() = default;

      //! Compute the 14-bit frequency value to send to the OPS
      uint32_t computeOpsFreq14(uint16_t voice_pitch14_, int16_t pitch_mod12_) const
      {
         int32_t pitch_14 = pitch_ratio14;

         if (pitch_fixed)
            pitch_14 -= 0x1000;
         else
            pitch_14 += voice_pitch14_;

         pitch_14 += detune4 + pitch_mod12_;

         return pitch_14 & 0x3FFF;
      }

      EnvGen*  env_gen{};
      uint16_t pitch_ratio14{0};
      bool     pitch_fixed{false};
      int8_t   detune4{0};
      uint8_t  rate_scale3{0};
   };

   // State representing EGS registers
   uint16_t   voice_pitch14{0};
   int16_t    pitch_mod12{0x0};                      // DX7 @ 0x30F2
   EgsOpState op[NUM_OP];
};
