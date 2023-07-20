
// Explore DX7 firmware translation

#include <cstdio>

#include "Table_dx7_rom_1.h"

#include "DX7_v1_8.h"

static Firmware fw{table_dx7_rom_1};

int main()
{
   // Load the first patch from the cartridge
   fw.patch_load_from_crt(0);

   for(unsigned note = 0; note < 128; note++)
   {
      fw.voice_convert_note_to_pitch(note);

      printf("%3d: 0x%02x%02x\n", note, fw.m_key_pitch, fw.m_key_pitch_low);
   }
}
