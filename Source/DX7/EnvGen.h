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

#include "Table_dx7_exp_14.h"
#include "Table_dx7_rate_30.h"

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

   void setOpLevel(uint8_t op_level_)
   {
      op_level = op_level_;

      for(unsigned index = 0; index < 4; index++)
      {
         Index p = index == 3 ? RELEASE : Index(index);

         unsigned level = (patch_level[index] << 1) + op_level;
         if (level > 0x7F)
            level = 0x7F;

         phase[p].level = 0x3f800000 - (level << 23);
      }
   }

   void setRate(unsigned index, uint8_t rate6_)
   {
      Index p = index == 3 ? RELEASE : Index(index);

      phase[p].rate = table_dx7_rate_30[rate6_];
   }

   void setLevel(unsigned index, uint8_t level6_)
   {
      patch_level[index] = level6_;
   }

   void setAmpMod(uint8_t amp_mod_)
   {
      amp_mod = amp_mod_;
   }

   void setAmpModSens(uint8_t amp_mod_sens_)
   {
      amp_mod_sens = amp_mod_sens_;
   }

   //! Start a note
   void keyOn()
   {
      phase[SUSTAIN].level = phase[DECAY2].level;
      phase[END].level     = phase[RELEASE].level;

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

   //! Get amplitude sample
   uint32_t operator()()
   {
      if (output >= current.level)
      {
         output -= current.rate;
         if (output <= current.level)
         {
            output = current.level;
            nextPhase();
         }
      }
      else
      {
         output += current.rate * 4;
         if (output >= current.level)
         {
            output = current.level;
            nextPhase();
         }
      }

      return table_dx7_exp_14[output >> (30 - 14)];
   }

private:
   void nextPhase()
   {
      if ((index != SUSTAIN) && (index != END))
      {
         setPhase(Index(index + 1));
      }
   }

   //! Change current phase
   void setPhase(Index index_)
   {
      index = index_;

      current.rate  = phase[index].rate;
      current.level = phase[index].level;
   }

   struct Phase
   {
      int32_t rate{0};        //!< Rate phase
      int32_t level{0};       //!< Target level for phase
   };

   int32_t output{0};        //!< Current amplitude
   Phase   current{};        //!< Current phase control
   Index   index{};          //!< Current phase index
   Phase   phase[NUM_PHASE];
   uint8_t op_level;         //!< The OP level
   uint8_t amp_mod;          //!< Amplitude modulation
   uint8_t amp_mod_sens;     //!< Amplitude modulation sensitivity
   uint8_t patch_level[4];
};
