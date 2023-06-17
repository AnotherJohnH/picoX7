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

#include <cmath>

#include "Freq.h"
#include "Phase.h"
#include "Sample.h"

#include "Table_freq.h"

namespace SYN
{
   //! Base class for oscillator sources
   class Oscillator
   {
   public:
      using Phase_ = uint32_t;

      Oscillator() = default;

      //! Synchronize phase to zero
      void sync()
      {
         phase_accum = 0;
      }

      //! Set oscillator frequency (Hz)
      void setFreq(Freq freq)
      {
         phase_inc       = double(freq) * PHASE_INC_1HZ;
         recip_phase_inc = (uint64_t(1) << PHASE_BITS) / phase_inc;
      }

      //! Set oscillator tuning (cents) 100 is a semitone
      void setTune(signed cents_)
      {
         tune_cents = cents_;

         updateFreqFromNote();
      }

      //! Set oscillator frequency from MIDI note value
      void setNote(uint8_t midi_note)
      {
         static const unsigned NOTE_A4_MIDI = 69;

         note_cents = (midi_note - NOTE_A4_MIDI) * 100;

         updateFreqFromNote();
      }

      //! Sample the phase as an N bit unsigned number
      template <unsigned N>
      uint32_t getPhase() const
      {
         return phase_accum >> (PHASE_BITS - N);
      }

      //! Sample the phase as an N bit unsigned number
      template <unsigned N>
      uint32_t samplePhase()
      {
         Phase_ phase = phase_accum;
         phase_accum += phase_inc;
         return phase >> (PHASE_BITS - N);
      }

      //! Sample the phase as an N bit unsigned number
      template <unsigned N>
      uint32_t samplePhase(uint32_t& frac)
      {
         Phase_ phase = phase_accum;
         phase_accum += phase_inc;
         frac = phase << N;
         return phase >> (PHASE_BITS - N);
      }

   protected: public:
      Phase_ phase_accum {0};
      Phase_ phase_inc {0};
      Phase_ recip_phase_inc {0};

   private:
      void updateFreqFromNote()
      {
         static const double NOTE_A4_FREQ = 440.0;

#ifdef MTL_TARGET
         setFreq(NOTE_A4_FREQ);
#else
         setFreq(NOTE_A4_FREQ * pow(2, (note_cents + tune_cents) / 1200.0));
#endif
      }

      static const unsigned PHASE_BITS    = sizeof(Phase) * 8;
      static const Phase_   PHASE_INC_1HZ = (uint64_t(1) << PHASE_BITS) / SAMPLE_FREQ;

      signed tune_cents {0};
      signed note_cents {0};
   };
}
