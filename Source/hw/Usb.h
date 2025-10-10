//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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

//! pico micro USB : MIDI in and storage interface
class USBDevice
   : public MIDI::Interface
   , public MTL::USBDevice
{
public:
   USBDevice(uint16_t         device_id_,
             const char*      device_name_,
             STB::FileSystem& file_system_)
      : MTL::USBDevice("https://github.com/AnotherJohnH",
                       device_id_, PLT_BCD_VERSION, device_name_,
                       PLT_COMMIT)
      , storage_if{this, file_system_}
   {}

   bool empty() const override { return midi_if.empty(); }

   uint8_t rx() override { return midi_if.rx(); }

   void tx(uint8_t byte) override {}

   MTL::USBMidiInterface        midi_if{this};
   MTL::USBMassStorageInterface storage_if;
};

#endif

} // namespace hw
