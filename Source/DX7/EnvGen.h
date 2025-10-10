//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief DX Envelope Generator simulation

#pragma once

#include <cstdio>

#include "SysEx.h"

#include "Table_dx_exp_19.h"

//! DX envelope generator
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
   void setAtten8(unsigned index_, unsigned atten8_)
   {
      Index p = index_ == NUM_CONTROL - 1 ? RELEASE : Index(index_);

      // Assume  24 <= INTERNAL_BITS
      phase[p].atten = (atten8_ << (INTERNAL_BITS -  8)) |
                       (atten8_ << (INTERNAL_BITS - 16)) |
                       (atten8_ << (INTERNAL_BITS - 24));
   }

   void setRate6(unsigned index, uint8_t rate6_)
   {
      Index p = index == 3 ? RELEASE : Index(index);

      phase[p].rate = table_dx_exp_19[rate6_];
   }

   void setAmpMod(unsigned amp_mod_12_)
   {
      amp_mod_12 = amp_mod_12_;
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

   static const unsigned NUM_CONTROL = 4;

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
   uint32_t amp_mod_12{0};          //!< Amplitude modulation
   Index    index{};                //!< Current phase index
   Phase    phase[NUM_PHASE];
};
