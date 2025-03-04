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

// \brief USB MIDI Device

#pragma once

#include "STB/MIDIInterface.h"
#include "hw/Config.h"

#if defined(HW_MIDI_USB_DEVICE)
#include "MTL/USBMidiDevice.h"

#endif

namespace hw {

#if defined(HW_MIDI_USB_DEVICE)

//! pico micro USB : MIDI in
class MidiUSBDevice : public MIDI::Interface
{
public:
   MidiUSBDevice(MIDI::InstrumentBase& instrument_,
                 uint16_t              device_id_,
                 const char*           device_name_,
                 bool                  debug_ = false)
      : MIDI::Interface(instrument_, debug_)
      , device("https://github.com/AnotherJohnH",
               device_id_, PLT_BCD_VERSION, device_name_,
               PLT_COMMIT)
   {}

   bool empty() const override { return device.empty(); }

   uint8_t rx() override { return device.rx(); }

   void tx(uint8_t byte) override {}

   MTL::USBMidiDevice device;
   MTL::Usb           usb{device};
};

#endif

} // namespace hw
