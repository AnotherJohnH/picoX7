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

//! Base class for voices used with SynthBase
class VoiceBase
{
   enum State { MUTE, ON, OFF };

public:
   VoiceBase() = default;

   //! is voice unused
   bool isMute() const { return state == MUTE; }

   //! is note "pressed"
   bool isOn()   const { return state == ON; }

   //! is note "released"
   bool isOff()  const { return state == OFF; }

   //! Get current MIDI pitch
   uint8_t getNote() const { return note; }

   //! Silence voice
   void mute()
   {
      state = MUTE;
      level = 0;

      gateOff();
   }

   //! MIDI note on event for this voice
   void noteOn(uint8_t note_, uint8_t velocity_)
   {
      state = ON;
      note  = note_;
      level = velocity_;

      gateOn();
   }

   //! MIDI note off event for this voice
   void noteOff(uint8_t velocity_)
   {
      state = OFF;
      level = velocity_;

      gateOff();
   }

   //! Update MIDI pitch bend
   void setPitchBend(int16_t value_)
   {
      pitch = value_;

      updatePitch();
   }

   //! Update voice level from MIDI aftertouch
   void setPressure(uint8_t value_)
   {
      level = value_;

      updateLevel();
   }

   //! Update MIDI controls
   void setControl(uint8_t number_, uint8_t value_)
   {
      control[number_] = value_;

      updateControl(number_, value_);
   }

protected:
   // Override for voice implementation
   virtual void gateOn() = 0;
   virtual void gateOff() = 0;
   virtual void updatePitch() {}
   virtual void updateLevel() {}
   virtual void updateControl(uint8_t number_, uint8_t value_) {}

   State   state {MUTE};
   int16_t pitch {0};
   uint8_t note {0};
   uint8_t level {0};
   uint8_t control[128] = {0};
};
