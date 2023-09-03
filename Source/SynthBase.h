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

#include <cstdint>

#include "STB/MIDIInstrument.h"

#include "SynthIO.h"

//! Base class for MIDI synth
template <unsigned N, typename VOICE, unsigned AMP_N = N>
class SynthBase
   : public MIDI::Instrument
   , public SynthIO
{
public:
   SynthBase()
      : MIDI::Instrument(N)
   {
      voice[0].setDebug(true);
   }

   //! Get next sample
   int32_t operator()()
   {
      int32_t mix {0};

      for(unsigned i = 0; i < N; ++i)
      {
         VOICE& v = voice[i];

         if (not v.isMute())
            mix += v();
      }

      return mix / AMP_N;
   }

   //! Control tick
   void tick()
   {
      for(unsigned i = 0; i < N; ++i)
      {
         VOICE& v = voice[i];

         if (not v.isMute())
            v.tick();
      }
   }

   // MIDI::Instrument implementation

   signed allocVoice() const override
   {
      unsigned first_decay       = order;
      unsigned first_decay_index = 0;
      unsigned first_on          = order;
      unsigned first_on_index    = 0;

      for(unsigned i = 0; i < N; ++i)
      {
         const VOICE& v = voice[i];

         if (v.isMute())
         {
            // Free voice found
            return i;
         }
         else if (v.isOff())
         {
            // Find oldest voice that is OFF
            if (v.getOrder() < first_decay)
            {
               first_decay       = v.getOrder();
               first_decay_index = i;
            }
         }
         else
         {
            // Find oldest voice that is ON
            if (v.getOrder() < first_on)
            {
               first_on       = v.getOrder();
               first_on_index = i;
            }
         }
      }

      // No free voices
      if (first_decay != order)
      {
         return first_decay_index;
      }

      // => Use the oldest voice that is on
      return first_on_index;
   }

   signed findVoice(uint8_t note_) const override
   {
      for(unsigned i = 0; i < N; ++i)
      {
         if (voice[i].isOn() && (voice[i].getNote() == note_))
         {
            return i;
         }
      }

      return -1;
   }

   void voiceMute(unsigned index_) override
   {
      voice[index_].noteOff(/* velocity */ 0, order++);
      voice[index_].mute();
   }

   void voiceOn(unsigned index_, uint8_t note_, uint8_t velocity_) override
   {
      voice[index_].noteOn(note_, velocity_, order++);
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      voice[index_].noteOff(velocity_, order++);
   }

   void voicePressure(unsigned index_, uint8_t level_) override
   {
      voice[index_].setPressure(level_);
   }

   void voiceControl(unsigned index_, uint8_t control_, uint8_t value_) override
   {
      voice[index_].setControl(control_, value_);
   }

   void voicePitchBend(unsigned index_, int16_t value_) override
   {
      voice[index_].setPitchBend(value_);
   }

protected:
   VOICE voice[N];

private:
   unsigned order {0}; //!< Order of last MIDI event
};
