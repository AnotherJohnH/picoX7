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

#include <cstdio>
#include <cmath>

#include "SysEx.h"
#include "OpsAlg.h"
#include "Firmware.h"

#include "Table_dx7_rom_1.h"

int main()
{
   unsigned     patch_index {0};
   SysEx::Voice patch{ table_dx7_rom_1, patch_index};
   OpsAlg       hw;
   Firmware     fw{hw};

   fw.loadData(&patch);

   for(unsigned t = 0; t < 2 * (49096); t++)
   {
           if (t ==     0) { hw.keyOn(); }
      else if (t == 49096) { hw.keyOff(); }

      uint32_t amp14 = hw.getEgsAmp(0);
      printf("%u,0x%04x\n", t, amp14);
   }
}
