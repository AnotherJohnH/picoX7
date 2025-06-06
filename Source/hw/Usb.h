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

// \brief USB Device

#pragma once

#include "STB/MIDIInterface.h"
#include "hw/Config.h"

#if defined(HW_USB_DEVICE)
#include "MTL/USBMidiInterface.h"
#include "MTL/USBMassStorageInterface.h"
#endif

namespace hw {

#if defined(HW_USB_DEVICE)

//! pico micro USB : MIDI in
class USBDevice
   : public MIDI::Interface
   , public MTL::USBDevice
{
public:
   USBDevice(MIDI::Instrument& instrument_,
             uint16_t          device_id_,
             const char*       device_name_)
      : MIDI::Interface(instrument_)
      , MTL::USBDevice("https://github.com/AnotherJohnH",
                       device_id_, PLT_BCD_VERSION, device_name_,
                       PLT_COMMIT)
   {}

   bool empty() const override { return midi_if.empty(); }

   uint8_t rx() override { return midi_if.rx(); }

   void tx(uint8_t byte) override {}

   MTL::USBMidiInterface midi_if{this};
};

#endif

} // namespace hw
