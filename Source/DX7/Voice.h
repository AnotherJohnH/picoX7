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
#include "Lfo.h"
#include "SysEx.h"

class Voice : public OpsAlg
{
public:
   Voice() = default;

   void setDebug(bool debug_) { debug = debug_; }

   void tick()
   {
      if (isComplete())
      {
         setMute();
      }

      lfo.tick();
   }

   //! Start a new note
   void gateOn() override
   {
      unsigned n14 = ((getNote() + 5) * 1024 + 400) / 12;

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

            init_phase_inc_32[i] = (table_dx7_exp_22[n14] * scale) << (13 - 8);

            init_phase_inc_32[i] += (op.osc_detune - 7) << 14;

            phase_inc_32[i] = init_phase_inc_32[i] + pitch_bend;
         }
      }
   }

   //! Release a new note
   void gateOff() override
   {
      for(unsigned i = 0; i < 6; ++i)
      {
         egs_amp[i].gateOff();
      }
   }

   void setLevel(uint8_t value) override
   {
      // TODO
   }

   void setControl(uint8_t control, uint8_t value) override
   {
      // TODO
   }

   void setPitch(int16_t value) override
   {
      // XXX needs to be added to the note value this is too late
      pitch_bend = (value << 8);

      for(unsigned i = 0; i < 6; ++i)
      {
         phase_inc_32[i] = init_phase_inc_32[i] + pitch_bend;
      }
   }

   void loadProgram(uint8_t number, const SysEx* ptr)
   {
      sysex = *ptr;

      if (debug)
      {
         printf("\nPROG %2u \n", number + 1);

         sysex.print();
      }

      OpsAlg::prog();
   }

private:
   bool debug{false};
   Lfo  lfo;
};
