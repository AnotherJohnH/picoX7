//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <unistd.h>
#include <cstdio>

#include "MTL/MTL.h"
#include "MTL/Pins.h"
#include "MTL/Digital.h"

void MTL_fault(unsigned excep_num_, uint32_t* fault_stack_ptr_)
{
   // Prevent console out using IRQs (as they won't work)
   MTL_nobuff();

   printf("\n\nFAULT: #%u\n", excep_num_, fault_stack_ptr_[6]);

   printf("R0  = %08x  R1 = %08x  R2 = %08x  R3 = %08x\n",
          fault_stack_ptr_[0], fault_stack_ptr_[1],
          fault_stack_ptr_[2], fault_stack_ptr_[3]);

   printf("R12 = %08x                 LR = %08x  PC = %08x\n",
          fault_stack_ptr_[4], fault_stack_ptr_[5],
          fault_stack_ptr_[6]);

   printf("PSR = %08x\n", fault_stack_ptr_[7]);

   MTL::Digital::Out<MTL::PIN_LED1> onboard_led;

   while(true)
   {
      for(unsigned i = 0; i < excep_num_; ++i)
      {
         onboard_led = true;
         usleep(100000);

         onboard_led = false;
         usleep(100000);
      }

      usleep(1000000);
   }
}

