#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_sine)

foreach(hw_config ${hw_config_list})

   add_executable(test_hw_${hw_config}
      Table_sine.cpp
      test_hw.cpp)

   target_include_directories(test_hw_${hw_config}
      PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
      )

   target_compile_definitions(test_hw_${hw_config} PRIVATE HW_${hw_config})

   target_link_libraries(test_hw_${hw_config} PRIVATE HW)

   set_target_properties(test_hw_${hw_config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
