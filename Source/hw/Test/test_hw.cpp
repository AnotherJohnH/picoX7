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

#include "hw/hw.h"

#include <cstdio>
#include <unistd.h>

enum Phase { DECL, INFO, START, RUN };

#define NOINLINE __attribute__ ((noinline))


//--- TEST ONBOARD LED ---------------------------------------------------------

static NOINLINE void testLED(Phase phase_)
{
   static hw::Led led{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("LED: (onboard) flash at 1Hz\n");
      break;

   case START:
      break;

   case RUN:
      led = not led;
      break;
   }
}

//--- TEST 16x2 LCD ------------------------------------------------------------

static NOINLINE void testLCD(Phase phase_)
{
#if not defined(HW_LCD_NONE)
   static hw::Lcd lcd{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("LCD: display \"Hello World!\", an incrementing value and an up arrow\n");
      break;

   case START:
      {
         uint8_t ch[8] =
         {
            0b00000,
            0b00100,
            0b01110,
            0b10101,
            0b00100,
            0b00100,
            0b00100,
            0b00000
         };

         lcd.progChar(0, ch);
      }
      break;

   case RUN:
      {
         static unsigned n{0};

         char text[16];
         snprintf(text, sizeof(text), "Hello %u", n++);

         lcd.move(0, 0);
         lcd.print(text);

         lcd.move(0, 1);
         lcd.print("World!");

         lcd.move(15, 1);
         lcd.putchar('\0');
      }
      break;
   }
#endif
}

//--- TEST PHYSICAL MIDI ------------------------------------------------------

static NOINLINE void testPhysMIDI(Phase phase_)
{
   static hw::PhysMidi phys_midi{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("MIDI: Laggy log of MIDI messages on the console\n" );
      break;

   case START:
      phys_midi.setDebug(true);
      break;

   case RUN:
      phys_midi.tick();
      break;
   }
}


//--- TEST USB INTERFACES -----------------------------------------------------

#include "STB/FAT/FAT16.h"

class FileSystem: public STB::FAT16<6>
{
public:
   FileSystem()
      : STB::FAT16<6>("HW_TEST")
   {
      static const char* readme_txt = "Hello, world!";

      addFile("README.txt", strlen(readme_txt), (uint8_t*)readme_txt);
   }
};

static hw::USBDevice* usb_ptr{nullptr};

extern "C" void IRQ_USBCTRL()
{
   if (usb_ptr != nullptr)
   {
      usb_ptr->irq();
   }
}

//! USB MIDI test
static NOINLINE void testUsb(Phase phase_)
{
   static FileSystem    file_portal{};
   static hw::USBDevice usb{0x91C0, "test_hw", file_portal};

   switch(phase_)
   {
   case DECL:
      usb_ptr = &usb;
      break;

   case INFO:
      printf("USB: Mass storage device \"HW_TEST\" with a simple README\n" );
      printf("USB: Laggy log of MIDI messages on the console\n" );
      break;

   case START:
      usb.setDebug(true);
      break;

   case RUN:
      usb.tick();
      break;
   }
}


//------------------------------------------------------------------------------

static void test(Phase phase_)
{
   if (1) testLED(phase_);
   if (1) testLCD(phase_);
   if (1) testPhysMIDI(phase_);
   if (1) testUsb(phase_);
}

//------------------------------------------------------------------------------

static void consoleReport()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("TEST HARDWARE\n");
   printf("\n");
   printf("Program  : Test HW (%s)\n", HW_DESCR);
   printf("Author   : Copyright (c) 2025 John D. Haughton\n");
   printf("License  : MIT\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);

   char config[1024];
   MTL::config.format(config, sizeof(config));
   puts(config);
}

//------------------------------------------------------------------------------

int MTL_main()
{
   test(DECL);

   consoleReport();

   printf("INFO\n\n");

   test(INFO);

   printf("\nSTART\n");

   test(START);

   printf("RUN\n\n");

   while(true)
   {
      test(RUN);

      usleep(500000);
   }

   return 0;
}
