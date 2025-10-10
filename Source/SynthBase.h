//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "STB/MIDIInstrument.h"

#include "SynthIO.h"

//! Base class for MIDI synth
template <unsigned N, typename VOICE, unsigned AMP_N = N>
class SynthBase : public MIDI::Instrument
{
public:
   SynthBase()
      : MIDI::Instrument(N)
   {}

   //! Get next sample
   int32_t getSample(unsigned first_voice_= 0,
                     unsigned num_voices_ = N)
   {
      int32_t mix {0};

      for(unsigned i = first_voice_; i < num_voices_; ++i)
      {
         VOICE& v = voice[i];

         if (not v.isMute())
            mix += v();
      }

      return mix / AMP_N;
   }

   //! Get next pair of samples
   int32_t getSamplePair(unsigned first_voice_ = 0,
                         unsigned last_voice_  = N)
   {
      int32_t mix1 {0};
      int32_t mix2 {0};

      for(unsigned i = first_voice_; i < last_voice_; ++i)
      {
         VOICE& v = voice[i];

         if (not v.isMute())
         {
            mix1 += v();
            mix2 += v();
         }
      }

      mix1 /= AMP_N;
      mix2 /= AMP_N;

      return (mix1 << 16) | (mix2 & 0xFFFF);
   }

   //! Control tick
   void tick(unsigned first_voice_ = 0,
             unsigned last_voice_  = N)
   {
      for(unsigned i = first_voice_; i < last_voice_; ++i)
      {
         VOICE& v = voice[i];

         if (not v.isMute())
            v.tick();
      }
   }

private:
   // MIDI::Instrument implementation
   void voiceMute(unsigned index_) override
   {
      voice[index_].noteOff(/* velocity */ 0);
      voice[index_].mute();
   }

   void voiceOn(unsigned index_, uint8_t note_, uint8_t velocity_) override
   {
      voice[index_].noteOn(note_, velocity_);
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      voice[index_].noteOff(velocity_);
   }

   void voicePressure(unsigned index_, uint8_t level_) override
   {
      voice[index_].setPressure(level_);
   }

   void voiceControl(unsigned index_, uint8_t control_, uint8_t value_) override
   {
      if (control_ == 119)
      {
         // Hack to allow program selection via CC119 for DAWs that
         // charge extra for easy MIDI program selection
         this->voiceProgram(index_, value_);
         return;
      }

      voice[index_].setControl(control_, value_);
   }

   void voicePitchBend(unsigned index_, int16_t value_) override
   {
      voice[index_].setPitchBend(value_);
   }

protected:
   VOICE   voice[N];
   SynthIO io{};
};
