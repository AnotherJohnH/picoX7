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

   bool isMute() const { return state == MUTE; }
   bool isOn()   const { return state == ON; }
   bool isOff()  const { return state == OFF; }

   //! Get current note pitch
   uint8_t getNote() const { return note; }

   //! Get current note velocity
   uint8_t getVelocity() const { return velocity; }

   //! Get event order of current state
   unsigned getOrder() const { return order; }

   //! MIDI note on event for this voice
   void noteOn(uint8_t note_, uint8_t velocity_, unsigned order_)
   {
      state    = ON;
      order    = order_;
      note     = note_;
      velocity = velocity_;

      gateOn();
   }

   //! MIDI note off event for this voice
   void noteOff(uint8_t velocity_, unsigned order_)
   {
      state    = OFF;
      order    = order_;
      velocity = velocity_;

      gateOff();
   }

   // Override for voice implementation

   virtual void mute() { state = MUTE; }
   virtual void gateOn() {}
   virtual void gateOff() {}
   virtual void setPressure(uint8_t value) {}
   virtual void setControl(uint8_t control, uint8_t value) {}
   virtual void setPitchBend(int16_t value) {}

private:
   State    state {MUTE};
   unsigned order {};     //!< Event order on entry into current state
   uint8_t  note {};
   uint8_t  velocity {};
};
