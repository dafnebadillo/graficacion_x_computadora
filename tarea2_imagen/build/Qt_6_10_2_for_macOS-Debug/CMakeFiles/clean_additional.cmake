# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/tarea2_imagen_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/tarea2_imagen_autogen.dir/ParseCache.txt"
  "tarea2_imagen_autogen"
  )
endif()
