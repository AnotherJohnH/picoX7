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

#include "Table_dx7_exp_22.h"

class Voice : public VoiceBase
{
public:
   Voice() = default;

   void setDebug(bool debug_) { debug = debug_; }

   void loadProgram(uint8_t number, const SysEx* ptr)
   {
      sysex = *ptr;

      if (debug)
      {
         printf("\nPROG %2u \n", number + 1);

         ptr->print();
      }

      pitch_env.prog(ptr->eg_pitch, 99);

      egs_ops.prog(ptr);
   }

   void tick()
   {
      if (egs_ops.isComplete())
      {
         setMute();
      }

      lfo.tick();
   }

   //! Start a new note
   void gateOn() override
   {
      // Compute note value (1-octave is 1024)
      unsigned n14 = (getNote() * 1024 / 12) + tune;

      for(unsigned i = 0; i < 6; ++i)
      {
         SysEx::Op& op = sysex.op[5 - i];

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

            egs_ops.setFreq(i, init_phase_inc_32[i] + pitch_bend);
         }
      }

      egs_ops.keyOn();
      lfo.keyOn();
   }

   //! Release a new note
   void gateOff() override
   {
      egs_ops.keyOff();
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
         egs_ops.setFreq(i, init_phase_inc_32[i] + pitch_bend);
      }
   }

   //! Return next sample for this voice
   int32_t operator()()
   {
      return egs_ops();
   } 

private:
   bool     debug{false};
   unsigned tune {460};             // A4 is close 440 Hz TODO re-check
   int16_t  pitch_bend {0};
   uint32_t init_phase_inc_32[6] = {0};
   EnvGen   pitch_env;
   Lfo      lfo;
   OpsAlg   egs_ops;
   SysEx    sysex;
};
