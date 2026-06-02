# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RayTracingShaders_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RayTracingShaders_autogen.dir/ParseCache.txt"
  "RayTracingShaders_autogen"
  )
endif()
