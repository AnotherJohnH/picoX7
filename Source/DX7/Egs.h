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

//! Model of Yamaha YM21290 EGS
template <unsigned NUM_OP>
class Egs
{
public:
   Egs() = default;

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

   //! Re-program voice from SysEx
   void prog(const SysEx::Voice* patch)
   {
      for(unsigned i = 0; i < NUM_OP; i++)
      {
         const SysEx::Op& op = patch->op[5 - i];

         egs[i].prog(op.eg_amp, op.out_level);
      }
   }

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

public:
   // EGS program state
   EnvGen   egs[NUM_OP];
   uint8_t  egs_op_rate_scaling[NUM_OP] = {0};
   uint8_t  egs_op_amp_mod_sens[NUM_OP] = {0};
   bool     egs_op_pitch_fixed[NUM_OP] = {false};
   uint16_t egs_op_pitch[NUM_OP] = {0};
   int8_t   egs_op_detune[NUM_OP] = {0};

   // EGS voice state
   uint8_t  egs_op_levels[NUM_OP] {0};
   uint16_t egs_amp_mod {0};
   uint16_t egs_voice_pitch {0};
   uint16_t egs_pitch_mod {0};

protected:
   int32_t getEgsAmp(unsigned index)
   {
      return egs[index]();
   }
};
