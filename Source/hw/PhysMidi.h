//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// \brief MIDI-in configuration

#pragma once

#include "STB/MIDIInterface.h"
#include "hw/Config.h"

#if defined(HW_MIDI_IN_UART)
#include "MTL/chip/Uart.h"

#elif defined(HW_MIDI_IN_NATIVE)
#include "PLT/MIDIInterface.h"

#endif

namespace hw {

#if defined(HW_MIDI_IN_UART)

//! Physical MIDI in
class PhysMidi : public MIDI::Interface
{
public:
   PhysMidi()
   {
      MTL::config.gpio(HW_MIDI_IN_UART::RX, "UART RX (MIDI in)");
   }

   bool empty() const override { return uart.empty(); }

   uint8_t rx() override { return uart.rx(); }

   void tx(uint8_t byte) override {}

private:
   HW_MIDI_IN_UART uart{/* baud */      31250,
                        /* bits */      8,
                        /* parity */    MTL::UART::NONE,
                        /* stop bits */ 1,
                        /* pull up */   true};
};

#elif defined(HW_MIDI_IN_FAKE)

//! Fake MIDI in with hard coded messages
class PhysMidi : public MIDI::Interface
{
public:
   PhysMidi() = default;

private:
   bool empty() const override { return n == sizeof(data); }

   uint8_t rx() override { return data[n++]; }

   void tx(uint8_t byte) override {}

   unsigned n {0};

   uint8_t  data[7] =
   {
      0x90,        // NOTE ON
      0x3C, 0x7F,  // C vel=127
      0x40, 0x7F,  // E vel=127
      0x43, 0x7F   // G vel=127
   };
};

#elif defined(HW_MIDI_IN_NATIVE)

using PhysMidi = PLT::MIDI::Interface;

#endif

} // namespace hw
