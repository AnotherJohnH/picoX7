#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(DX7)

foreach(hw_config ${hw_picoX7_config_list})

   add_executable(picoX7_${hw_config} picoX7.cpp)

   target_compile_definitions(picoX7_${hw_config} PRIVATE HW_${hw_config})

   target_link_libraries(picoX7_${hw_config} PRIVATE DX7 HW)

   set_target_properties(picoX7_${hw_config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
