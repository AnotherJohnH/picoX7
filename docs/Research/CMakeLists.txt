#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL native)
   add_executable(exp_dx7 exp_dx7.cpp)

   target_link_libraries(exp_dx7 PRIVATE DX7 PLT STB)
endif()

