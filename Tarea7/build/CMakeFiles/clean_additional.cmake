# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Tarea7_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Tarea7_autogen.dir/ParseCache.txt"
  "Tarea7_autogen"
  )
endif()
