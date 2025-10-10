//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cstring>

#include "STB/FAT/FAT16.h"

#include "hw/hw.h"

class FilePortal : public STB::FAT16<6>
{
public:
   FilePortal(const char* label_)
      : STB::FAT16<6>(label_)
   {
      addFile("INDEX.htm", strlen(index_html), (uint8_t*)index_html);
   }

   //! Auto-generate the project README
   const char* genREADME()
   {
      char* s     = readme;
      char* end_s = readme + sizeof(readme);

      s += snprintf(s, end_s - s, "Program  : picoX7 (%s)\n", HW_DESCR);
      s += snprintf(s, end_s - s, "Author   : Copyright (c) 2023 John D. Haughton\n");
      s += snprintf(s, end_s - s, "License  : MIT\n");
      s += snprintf(s, end_s - s, "Version  : %s\n", PLT_VERSION);
      s += snprintf(s, end_s - s, "Commit   : %s\n", PLT_COMMIT);
      s += snprintf(s, end_s - s, "Built    : %s %s\n", __TIME__, __DATE__);
#if defined(__clang__)
      s += snprintf(s, end_s - s, "Compiler : Clang %s\n", __VERSION__);
#elif defined(__GNUC__)
      s += snprintf(s, end_s - s, "Compiler : GCC %s\n", __VERSION__);
#else
      s += snprintf(s, end_s - s, "Compiler : %s\n", __VERSION__);
#endif

#if not defined(HW_NATIVE)
      s += MTL::config.format(s, end_s - s);
#endif

      addFile("README.txt", s - readme, (uint8_t*)readme);

      return readme;
   }

private:
   char readme[1024];

   const char* index_html =
      "<html>"
         "<head>"
            "<meta http-equiv=\"refresh\" content=\"0;URL='https://github.com/AnotherJohnH/picoX7'\"/>"
         "</head>"
         "<body>"
            "Redirecting to <a href='https://github.com/AnotherJohnH/picoX7'>github.com</a>"
         "</body>"
      "</html>";
};
