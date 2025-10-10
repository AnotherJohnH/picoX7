#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_dx_exp_14)
autogen_py(Table_dx_exp_19)
autogen_py(Table_dx_exp_32)
autogen_py(Table_dx_log_sine_14)
autogen_py(Table_dx7_rom_1 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom1a.syx)
autogen_py(Table_dx7_rom_2 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom2a.syx)
autogen_py(Table_dx7_rom_3 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom3a.syx)
autogen_py(Table_dx7_rom_4 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom4a.syx)

add_library(DX7 STATIC
   SysEx.cpp
   Table_dx_exp_14.cpp
   Table_dx_exp_19.cpp
   Table_dx_exp_32.cpp
   Table_dx_log_sine_14.cpp
   Table_dx7_rom_1.cpp
   Table_dx7_rom_2.cpp
   Table_dx7_rom_3.cpp
   Table_dx7_rom_4.cpp
   )

target_include_directories(DX7
   PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
   )

add_subdirectory(test)
