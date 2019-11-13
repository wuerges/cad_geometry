cmake_minimum_required(VERSION 3.0)
project(cad_geometry CXX)
set (CMAKE_CXX_STANDARD 17)

add_library(cadgeometry
  src/base.cxx  
  src/rtree_adapter.cxx
  )
include_directories(include)

IF(EXISTS "${PROJECT_SOURCE_DIR}/ext-libs/rapidcheck/CMakeLists.txt")
  add_subdirectory("ext-libs/rapidcheck")
  add_subdirectory(test)
ENDIF()

