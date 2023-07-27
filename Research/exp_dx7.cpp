
// Explore DX7 firmware translation

#include <cstdio>

#include "DX7_v1_8.h"

#include "Table_dx7_rom_1.h"
#include "../Source/DX7/SysEx.h"

static Firmware fw{table_dx7_rom_1};

void sweep_lfo_speed()
{
   for(unsigned value = 0; value < 99; ++value)
   {
      fw.m_patch_current_lfo_speed = value;
      fw.patch_load_lfo();
      double freq = 375.0 * fw.m_lfo_phase_increment / 0x10000;
      printf("%u,%u,%.2f Hz\n", value, fw.m_lfo_phase_increment, freq);
   }
}

void sweep_lfo_delay()
{
   for(unsigned value = 0; value < 99; ++value)
   {
      fw.m_patch_current_lfo_delay = value;
      fw.patch_load_lfo();
      double delay = double(0x10000) / (fw.m_lfo_delay_increment * 375);
      printf("%u,%u,%.2f s\n", value, fw.m_lfo_delay_increment, delay);
   }
}

void sweep_note()
{
   for(unsigned note = 0; note < 128; note++)
   {
      fw.voice_convert_note_to_pitch(note);
      printf("%3d: 0x%02x%02x\n", note, fw.m_key_pitch, fw.m_key_pitch_low);
   }
}

void sample_lfo()
{
   fw.m_lfo_delay_counter        = 0;
   fw.m_lfo_fade_in_scale_factor = 0;
   fw.m_lfo_phase_accumulator    = 0x7FFF;

   for(unsigned i = 0; i < 1000; i++)
   {
      fw.lfo_get_amplitude();
      printf("%u,0x%x,0x%x,%d\n",
             i,
             fw.m_lfo_delay_counter,
             fw.m_lfo_fade_in_scale_factor,
             fw.m_lfo_curr_amplitude);
   }
}

int main()
{
   unsigned patch_number = 1;

   // Copy in the first patch from the cartridge
   fw.patch_load_from_crt(patch_number - 1);

   // Decode to console to confirm deserialisation
   SysEx::Voice* voice = (SysEx::Voice*)fw.m_patch_current_buffer;
   printf("\n");
   voice->print(patch_number);

   // Load the patch
   fw.patch_load_data();

   // Dump LFO parameters derived from the patch
   printf("\n");
   printf("LFO delay inc %04x\n", fw.m_lfo_delay_increment);
   printf("LFO phase inc %04x\n", fw.m_lfo_phase_increment);
   printf("LFO PMD       %02x\n", fw.m_lfo_pitch_mod_depth);
   printf("LFO AMD       %02x\n", fw.m_lfo_amp_mod_depth);
   printf("LFO waveform  %u\n",   fw.m_lfo_waveform);
   printf("LFO PMS       %02x\n", fw.m_lfo_pitch_mod_sens);

   //sweep_note();
   //sweep_lfo_delay();
   //sweep_lfo_speed();
   //sample_lfo();
}
