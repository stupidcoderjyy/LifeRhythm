# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\LifeRhythm_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LifeRhythm_autogen.dir\\ParseCache.txt"
  "LifeRhythm_autogen"
  )
endif()
