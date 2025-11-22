#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(DX7)

add_picoX7_executable(picoX7 picoX7.cpp LIBRARIES DX7)
