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

#include "SysEx.h"

#include "Table_dx7_exp_14.h"
#include "Table_dx7_level_30.h"
#include "Table_dx7_rate_30.h"

//! DX7 envelope generator
class EnvGen
{
   enum Index : uint8_t
   {
      ATTACK = 0, DECAY1 = 1, DECAY2 = 2, SUSTAIN = 3, RELEASE = 4, IDLE = 5, NUM_PHASE
   };

public:
   EnvGen() = default;

   //! Program the level and rate
   void prog(const SysEx::EnvGen& env, uint8_t out_level)
   {
      for(unsigned i = 0; i < 4; i++)
      {
         Index p;

         switch(i)
         {
         case 0: p = ATTACK;  break;
         case 1: p = DECAY1;  break;
         case 2: p = DECAY2;  break;
         case 3: p = RELEASE; break;
         }

         phase[p].rate  = table_dx7_rate_30[env.rate[i]];
         phase[p].level = table_dx7_level_30[env.level[i] * out_level / 100];

         unsigned prev_i = (i - 1) & 3;

         if (env.level[i] > env.level[prev_i])
         {
             // Seems like attack needs to be 4 times faster
             phase[p].rate *= 4;
             phase[p].sign = +1;
         }
         else
         {
             phase[p].rate *= -1;
             phase[p].sign = -1;
         }
      }

      phase[SUSTAIN].rate  = 0;
      phase[SUSTAIN].level = phase[DECAY2].level;

      phase[IDLE].rate  = 0;
      phase[IDLE].level = phase[RELEASE].level;

      setPhase(IDLE);
   }

   //! Start a note
   void keyOn()
   {
      setPhase(ATTACK);
   }

   //! Release a note
   void keyOff()
   {
      if (index < RELEASE)
         setPhase(RELEASE);
   }

   //! Check if amplitude has reached L4
   bool isComplete() const { return index == IDLE; }

   //! Get amplitude sample
   uint32_t operator()()
   {
      ampl += current.rate;

      int32_t over = (ampl - current.level) * current.sign;
      if (over > 0)
      {
         ampl = current.level;

         setPhase(Index(index + 1));
      }

      return table_dx7_exp_14[ampl >> (30 - 14)];
   }

private:
   //! Change current phase
   void setPhase(Index index_)
   {
      index = index_;

      current.rate  = phase[index].rate;
      current.level = phase[index].level;
      current.sign  = phase[index].sign;
   }

   struct Phase
   {
      int32_t rate{0};  //!< Rate phase
      int32_t level{0}; //!< Target level for phase
      int32_t sign{0};  //!< Direction
   };

   int32_t ampl{0};          //!< Current amplitude
   Phase   current{};        //!< Current phase control
   Index   index{};          //!< Current phase index
   Phase   phase[NUM_PHASE];
};
