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
   enum Phase : uint8_t
   {
      P1 = 0, P2 = 1, P3 = 2, SUSTAIN = 3, P4 = 4, RELEASE = 4, COMPLETE = 5
   };

public:
   EnvGen() = default;

   //! Check if amplitude has reached L4
   bool isComplete() const { return phase == COMPLETE; }

   //! Program the level and rate
   void prog(const SysEx::EnvGen& env, uint8_t out_level)
   {
      for(unsigned i = 0; i < 4; i++)
      {
         Phase p = i == 3 ? P4 : Phase(i);

         L[p] = table_dx7_level_30[env.level[i] * out_level / 100];
         R[p] = table_dx7_rate_30[env.rate[i]];
      }

      L[SUSTAIN] = L[P3];
      R[SUSTAIN] = 0;

      L[COMPLETE] = L[P4];
      R[COMPLETE] = 0;
   }

   //! Start a note
   void gateOn()
   {
      ampl = L[P4];
      setPhase(P1);
   }

   //! Release a note
   void gateOff()
   {
      setPhase(RELEASE);
   }

   //! Get amplitude sample
   uint32_t operator()()
   {
      ampl += rate;

      if (rise == (ampl >= level))
      {
         ampl = level;
         setPhase(Phase(phase + 1));
      }

      return table_dx7_exp_14[ampl >> (30 - 14)];
   }

private:
   //! Change current phase
   void setPhase(Phase phase_)
   {
      phase = phase_;
      level = L[phase];
      rise  = ampl < level;
      rate  = rise ? +R[phase]
                   : -R[phase];

      if (rate > 0)
         // Seems like attack needs to be 4 times faster
         rate *= 4;
   }

   int32_t  ampl{0};     //!< Current amplitude
   int32_t  rate{0};     //!< Rate for current phase
   int32_t  level{0};    //!< Target level for current phase
   Phase    phase{P1};   //!< Current phase
   bool     rise{false}; //!< Direction of change in current phase
   uint32_t R[6] = {};   //!< Programmed rates for each phase
   uint32_t L[6] = {};   //!< programmed levels for each phase
};
