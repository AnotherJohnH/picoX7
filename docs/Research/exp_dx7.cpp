
// Explore DX7 firmware translation

#include <cstdio>
#include <cmath>

#include "DX7_v1_8.h"

#include "Table_dx7_rom_1.h"
#include "../Source/DX7/SysEx.h"

static AsmFirmware fw{table_dx7_rom_1};

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

      printf("%3d: 0x%04x\n", note, fw.m_key_pitch);
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

// mode 0 => ratio, 1 => fixed
void sweep_operator_pitch(unsigned mode, unsigned fine)
{
   for(uint8_t coarse = 0; coarse < 32; ++coarse)
   {
      fw.m_patch_current_buffer[17] = mode;
      fw.m_patch_current_buffer[18] = coarse;
      fw.m_patch_current_buffer[19] = fine;

      fw.patch_load_data();

      if (mode == 0)
      {
         signed value = fw.p_egs_op_pitch[0] - 0x232c;
         double ratio = pow(2, value / 4096.0);
 
         printf("mode=0 coarse=%2u fine=%2u P_EGS_OP_PITCH=0x%04X ratio=%.3f\n",
                coarse, fine, fw.p_egs_op_pitch[0], ratio);
      }
      else
      {
         signed value = fw.p_egs_op_pitch[0] - 0x16ac;
         double freq  = pow(2, value / 4096.0);

         printf("mode=1 coarse=%2u fine=%2u P_EGS_OP_PITCH=0x%04X freq=%.3f\n",
                coarse, fine, fw.p_egs_op_pitch[0], freq);

         if (coarse == 3) break;
      }
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

   printf("\n");
   for(unsigned op = 0; op < 6; ++op)
   {
      printf("P_EGS_OP_PITCH[%u] = 0x%04x\n", op, fw.p_egs_op_pitch[op]);
   }

   printf("\n");
   for(unsigned op = 0; op < 6; ++op)
   {
      printf("P_EGS_OP_DETUNE[%u] = 0x%x\n", op, fw.p_egs_op_detune[op]);
   }

   //sweep_note();
   //sweep_lfo_delay();
   //sweep_lfo_speed();
   //sample_lfo();
   //sweep_operator_pitch(/* mode */ 0, /* fine */ 0);
   //sweep_operator_pitch(/* mode */ 0, /* fine */ 50);
   //sweep_operator_pitch(/* mode */ 0, /* fine */ 99);

   sweep_operator_pitch(/* mode */ 1, /* fine */ 0);
   sweep_operator_pitch(/* mode */ 1, /* fine */ 50);
   sweep_operator_pitch(/* mode */ 1, /* fine */ 99);
}
