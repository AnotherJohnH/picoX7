//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "DX7/Egs.h"

#include "DX7/SysEx.h"
#include "DX7/DX7Firmware.h"

#include "Table_dx7_rom_1.h"

#include <cstdio>
#include <cmath>

#include "STB/Test.h"

#define DEBUG if (0) printf

TEST(Egs, old_test)
{
   unsigned      patch_index{0};
   SysEx::Voice  patch{table_dx7_rom_1, patch_index};
   Egs           hw;
   DX7::Firmware fw{hw};

   fw.loadVoice(&patch);

   for(unsigned t = 0; t < 2 * (49096); t++)
   {
           if (t ==     0) { hw.keyOn(); }
      else if (t == 49096) { hw.keyOff(); }

      uint32_t amp14 = hw.getEgsAmp(0);
      DEBUG("%u,0x%04x\n", t, amp14);
   }
}
