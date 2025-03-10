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

// \brief DX7 EGS simulation - EG

#pragma once

#include <cstdio>

#include "SysEx.h"

#include "Table_dx7_exp_19.h"

//! DX7 envelope generator
class EnvGen
{
   enum Index : uint8_t
   {
      ATTACK = 0, DECAY1 = 1, DECAY2 = 2, SUSTAIN = 3, RELEASE = 4, END = 5, NUM_PHASE
   };

public:
   EnvGen()
   {
      phase[SUSTAIN].rate = 0;
      phase[END].rate     = 0;

      setPhase(END);
   }

   //! Set operator attenuation
   void setOpAtten(uint8_t op_atten_8_)
   {
      for(unsigned index = 0; index < 4; index++)
      {
         Index p = index == 3 ? RELEASE : Index(index);

         unsigned atten8 = patch_atten_8[index] + op_atten_8_;
         if (atten8 > 0xFF)
            atten8 = 0xFF;

         // Assume  24 <= INTERNAL_BITS
         phase[p].atten = (atten8 << (INTERNAL_BITS -  8)) |
                          (atten8 << (INTERNAL_BITS - 16)) |
                          (atten8 << (INTERNAL_BITS - 24));
      }
   }

   //! Set EG patch attenutaion
   void setAtten(unsigned index, uint8_t atten6_)
   {
      // Convert to 8-bit
      // 0x00 - No attenuation
      // 0xFF - Full attenuation

      // XXX "interpolating" LS 2 bits from top 2 bits (may not be accurate?)
      patch_atten_8[index] = (atten6_ << 2) | (atten6_ >> 4);
   }

   void setRate(unsigned index, uint8_t rate6_)
   {
      Index p = index == 3 ? RELEASE : Index(index);

      phase[p].rate = table_dx7_exp_19[rate6_];
   }

   void setAmpModSens(uint8_t amp_mod_sens_2_)
   {
      switch(amp_mod_sens_2_)
      {
      case 0b00: amp_mod_sens_12 = 0xFFF; break;
      case 0b01: amp_mod_sens_12 = 0xAAA; break;
      case 0b10: amp_mod_sens_12 = 0x555; break;
      case 0b11: amp_mod_sens_12 = 0x000; break;
      }
   }

   void setAmpMod(uint8_t amp_mod_8_)
   {
      amp_mod_12 = (amp_mod_8_ << 4) |
                   (amp_mod_8_ >> 4);

      amp_mod_12 -= amp_mod_sens_12;

      if (amp_mod_12 < 0)
         amp_mod_12 = 0;
   }

   //! Start a note
   void keyOn()
   {
      phase[SUSTAIN].atten = phase[DECAY2].atten;
      phase[END].atten     = phase[RELEASE].atten;
      attenuation          = phase[RELEASE].atten;

      setPhase(ATTACK);
   }

   //! Release a note
   void keyOff()
   {
      if (index < RELEASE)
         setPhase(RELEASE);
   }

   //! Check if amplitude has reached L4
   bool isComplete() const { return index == END; }

   //! Get amplitude attenuation sample 12-bit logarithmic
   //! 0x000 no attenuation
   //! 0xFFF full attenuation
   uint32_t getAtten12()
   {
      if (attenuation >= target.atten)
      {
         attenuation -= target.rate;
         if (attenuation <= target.atten)
         {
            attenuation = target.atten;
            nextPhase();
         }
      }
      else
      {
         attenuation += target.rate / 4;
         if (attenuation >= target.atten)
         {
            attenuation = target.atten;
            nextPhase();
         }
      }

      uint32_t atten_12 = attenuation >> (INTERNAL_BITS - OUTPUT_BITS);

      return atten_12 + amp_mod_12;
   }

   // For test
   uint32_t dbgInternal() const { return attenuation; }
   uint32_t dbgTarget() const { return target.atten; }
   uint32_t dbgRate() const { return target.rate; }
   unsigned dbgPhase() const { return unsigned(index); }

private:
   void nextPhase()
   {
      if ((index != SUSTAIN) && (index != END))
      {
         setPhase(Index(index + 1));
      }
   }

   //! Change target phase
   void setPhase(Index index_)
   {
      index  = index_;
      target = phase[index];
   }

   struct Phase
   {
      int32_t rate{0};  //!< Rate phase
      int32_t atten{0}; //!< Target attenuation for phase
   };

   static const unsigned INTERNAL_BITS = 24;
   static const unsigned OUTPUT_BITS   = 12;
   static const uint32_t MAX_ATTEN     = (1 << INTERNAL_BITS) - 1;

   int32_t  attenuation{MAX_ATTEN}; //!< Current attenuation (initialize to full attenuation)
   Phase    target{};               //!< Current target phase
   Index    index{};                //!< Current phase index
   Phase    phase[NUM_PHASE];
   int32_t  amp_mod_12{0};      //!< Amplitude modulation
   uint32_t amp_mod_sens_12{0}; //!< Amplitude modulation sensitivity
   uint8_t  patch_atten_8[4] = {};
};
