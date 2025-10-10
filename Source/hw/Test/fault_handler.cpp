//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

