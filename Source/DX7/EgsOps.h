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

#include "SYN/VoiceBase.h"

#include "Table_dx7_exp_14.h"
#include "Table_dx7_exp_22.h"
#include "Table_dx7_sine_15.h"
#include "Table_dx7_sine_div3_15.h"
#include "Table_dx7_sine_div5_15.h"

#include "EnvGen.h"

//! Model of YM21280 OPS and YM21290 EGS
class EgsOps : public VoiceBase
{
public:
   EgsOps() = default;

   //! Check if all amplitude envelopes are at L4
   bool isComplete() const
   {
      for(unsigned i = 0; i < 6; ++i)
      {
         if (not egs_amp[i].isComplete())
            return false;
      }

      return true;
   }

   //! Start a new note
   void gateOn()
   {
      unsigned f14 = ((getNote() + 5) * 1024 + 400) / 12;

      for(unsigned i = 0; i < 6; ++i)
      {
         SysEx::Op& op = sysex.op[5 - i];

         egs_amp[i].gateOn();

         if (sysex.osc_sync)
         {
            phase_accum_32[i] = 0;
         }

         if (op.osc_mode == SysEx::Op::FIXED)
         {
         }
         else
         {
            unsigned scale;

            if (op.osc_freq_coarse == 0)
            {
               scale = (100 + op.osc_freq_fine) * 128 / 100;
            }
            else
            {
               scale = op.osc_freq_coarse * (100 + op.osc_freq_fine) * 256 / 100;
            }

            phase_inc_32[i] = (table_dx7_exp_22[f14] * scale) << (13 - 8);

            phase_inc_32[i] += (op.osc_detune - 7) << 14;
         }
      }
   }

   //! Release a new note
   void gateOff()
   {
      for(unsigned i = 0; i < 6; ++i)
      {
         egs_amp[i].gateOff();
      }
   }

protected:
   //! Re-program voice from SysEx
   void prog()
   {
      for(unsigned i = 0; i < 6; i++)
      {
         const SysEx::Op& op = sysex.op[5 - i];

         egs_amp[i].prog(op.eg_amp, op.out_level);
      }

      fbl = (7 - sysex.feedback) + 5;
   }

   //! Simulate a single operator (and associated envelope generator)
   template <unsigned OP, unsigned SEL, bool A, bool C, bool D, unsigned COM>
   int32_t ops()
   {
      const unsigned i = OP - 1;

      int32_t amp_14 = egs_amp[i]();

      phase_accum_32[i] += phase_inc_32[i];

      uint32_t phase_12 = (phase_accum_32[i] + (modulation_12 << 22)) >> 20;

      int32_t output_15;
      switch (COM)
      {
      case 1: output_15 = (table_dx7_sine_15[phase_12]      * amp_14) >> 14; break; //  1/1
      case 2: output_15 = (table_dx7_sine_15[phase_12]      * amp_14) >> 15; break; //  1/2
      case 3: output_15 = (table_dx7_sine_div3_15[phase_12] * amp_14) >> 14; break; //  1/3
      case 4: output_15 = (table_dx7_sine_15[phase_12]      * amp_14) >> 16; break; //  1/4
      case 5: output_15 = (table_dx7_sine_div5_15[phase_12] * amp_14) >> 14; break; //  1/5
      case 6: output_15 = (table_dx7_sine_div3_15[phase_12] * amp_14) >> 15; break; //  1/6
      }

      int32_t sum_15 = 0;
      if (C) sum_15 = memory_15;
      if (D) sum_15 += output_15;

      switch(SEL)
      {
      case 0: modulation_12 = 0;                                    break;
      case 1: modulation_12 = output_15 >> 3;                       break;
      case 2: modulation_12 = sum_15 >> 3;                          break;
      case 3: modulation_12 = memory_15 >> 3;                       break;
      case 4: modulation_12 = feedback1_15 >> 3;                    break;
      case 5: modulation_12 = (feedback1_15 + feedback2_15) >> fbl; break;
      }

      if (A)
      {
         // Feeedback path enabled
         feedback2_15 = feedback1_15;
         feedback1_15 = output_15;
      }

      if (C or D)
      {
         // Memory write enable
         memory_15 = sum_15;
      }

      return sum_15 << 1;
   }

private:
   uint8_t  fbl {0};

   uint32_t phase_accum_32[6] = {0};
   uint32_t phase_inc_32[6]   = {0};

   int32_t  modulation_12 {0};
   int32_t  feedback1_15 {0};
   int32_t  feedback2_15 {0};
   int32_t  memory_15 {0};

   EnvGen   egs_amp[6];

protected:
   SysEx    sysex;
};
