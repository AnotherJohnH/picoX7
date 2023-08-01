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

#include "VoiceBase.h"

#include "Lfo.h"
#include "Firmware.h"
#include "SysEx.h"

#include "Table_dx7_exp_22.h"

class Voice : public VoiceBase
{
public:
   Voice() = default;

   void setDebug(bool debug_) { debug = debug_; }

   void loadProgram(uint8_t number, const SysEx::Voice* voice)
   {
      patch = *voice;

      if (debug)
      {
         voice->print(number + 1);
      }

      lfo.prog(patch);

      // pitch_env.prog(voice->eg_pitch, 99);

      fw.loadData(&patch);
   }

   void tick()
   {
      if (hw.isComplete())
      {
         mute();
      }

      lfo.tick();
   }

   //! Start a new note
   void gateOn() override
   {
      signed note = getNote() + patch.transpose - 24;
           if (note <   0) note = 0;
      else if (note > 127) note = 127;

      // Compute note value (1-octave is 1024)
      unsigned n14 = (note * 1024 / 12) + tune;

      for(unsigned op_index = 0; op_index < 6; ++op_index)
      {
         SysEx::Op& op = patch.op[op_index];

         if (op.osc_mode == SysEx::FIXED)
         {
            unsigned f8;

            switch(op.osc_freq_coarse & 0b11)
            {
            case 0b00: f8 =    1 << 8; break;
            case 0b01: f8 =   10 << 8; break;
            case 0b10: f8 =  100 << 8; break;
            case 0b11: f8 = 1000 << 8; break;
            }

            // XXX The following is a kludged and wrong as the
            //     the fine scaling is not linear
            static const unsigned SAMPLE_RATE = 49096;
            f8 += f8 * op.osc_freq_fine * 9 / 100;
            hw.setFreq(op_index, ((f8 << 14) / SAMPLE_RATE) << 10);
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

            init_phase_inc_32[op_index] = (table_dx7_exp_22[n14] * scale) << (13 - 8);

            init_phase_inc_32[op_index] += (op.osc_detune - 7) << 14;

            hw.setFreq(op_index, init_phase_inc_32[op_index] + pitch_bend);
         }
      }

      hw.keyOn();
      lfo.keyOn();
   }

   //! Release a new note
   void gateOff() override
   {
      hw.keyOff();
   }

   void setLevel(uint8_t value) override
   {
      aftertouch.value = value;
   }

   void setControl(uint8_t control, uint8_t value) override
   {
      switch (control)
      {
      case  1: modulation_wheel.value = value; break;
      case  2: breath_control.value   = value; break;
      case  4: foot_control.value     = value; break;

      case 64: /* sustain */ break;
      case 65: /* portamento */ break;
      }
   }

   void setPitch(int16_t value) override
   {
      // XXX needs to be added to the note value this is too late
      pitch_bend = (value << 8);

      for(unsigned op_index = 0; op_index < 6; ++op_index)
      {
         hw.setFreq(op_index, init_phase_inc_32[op_index] + pitch_bend);
      }
   }

   //! Return next sample for this voice
   int32_t operator()()
   {
      return hw();
   } 

private:
   bool          debug{false};
   unsigned      tune {460};             // A4 is close 440 Hz TODO re-check
   int16_t       pitch_bend {0};
   uint32_t      init_phase_inc_32[6] = {0};
   EnvGen        pitch_env;
   Lfo           lfo;
   OpsAlg        hw;
   Firmware      fw{hw};
   SysEx::Voice  patch;

   // Modulation sources

   struct Modulation
   {
      Modulation() = default;

      uint8_t value {0};
      uint8_t range {50};
      bool    pitch {true};
      bool    amplitude {false};
      bool    eg_bias {false};
   };

   Modulation modulation_wheel {};
   Modulation foot_control {};
   Modulation breath_control {};
   Modulation aftertouch {};
};
