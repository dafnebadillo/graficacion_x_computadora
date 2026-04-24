# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MiProyectoQt_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MiProyectoQt_autogen.dir/ParseCache.txt"
  "MiProyectoQt_autogen"
  )
endif()
