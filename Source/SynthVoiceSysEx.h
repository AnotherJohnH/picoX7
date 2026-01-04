//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SynthVoice.h"

template <typename VOICE, unsigned NUM_VOICES, unsigned MAX_SYSEX_SIZE>
class SynthVoiceSysEx : public SynthVoice<VOICE, NUM_VOICES>
{
public:
   SynthVoiceSysEx(uint8_t midi_manuf_id_)
      : MIDI_MANUF_ID(midi_manuf_id_)
   {
   }

protected:
   virtual void processSysEx() = 0;

   unsigned sysex_size{};
   uint8_t  sysex_buffer[MAX_SYSEX_SIZE];

private:
   void sysEx(uint8_t byte_) override
   {
      if (byte_ == 0xF0)
      {
         sysex_state = SYSEX_START;
         return;
      }
      else if (byte_ == 0xF7)
      {
         if ((sysex_state == SYSEX_CAPTURE) && (sysex_size > 0))
         {
            processSysEx();
         }
         sysex_state = SYSEX_IGNORE;
         return;
      }

      switch(sysex_state)
      {
      case SYSEX_IGNORE:
         break;

      case SYSEX_START:
         if (byte_ == MIDI_MANUF_ID)
         {
            sysex_size  = 0;
            sysex_state = SYSEX_CAPTURE;
         }
         break;

      case SYSEX_CAPTURE:
         if (sysex_size < MAX_SYSEX_SIZE)
         {
            sysex_buffer[sysex_size++] = byte_;
         }
         break;
      }
   }

   enum SysExState { SYSEX_IGNORE, SYSEX_START, SYSEX_CAPTURE };

   const uint8_t MIDI_MANUF_ID;

   SysExState sysex_state{SYSEX_IGNORE};
};
