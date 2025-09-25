# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\monInterface_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\monInterface_autogen.dir\\ParseCache.txt"
  "monInterface_autogen"
  )
endif()
