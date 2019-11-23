cmake_minimum_required(VERSION 3.15)
project(cad_geometry CXX)
set (CMAKE_CXX_STANDARD 17)


add_library(cadgeometry 
  # base.cxx
  # rtree_adapter.cxx
)
target_sources(cadgeometry
  PRIVATE
    src/base.cxx
    src/rtree_adapter.cxx
  PUBLIC
    include/geometry.hpp
    include/rtree_adapter.hpp
    include/RTree.h
    include/treap.hpp
)
target_include_directories(cadgeometry
  PUBLIC
    include
)

# add_library(cadgeometry
#   src/base.cxx  
#   src/rtree_adapter.cxx
#   )
# target_include_directories(cadgeometry PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR}/include/)

IF(EXISTS "${PROJECT_SOURCE_DIR}/ext-libs/rapidcheck/CMakeLists.txt")
  add_subdirectory("ext-libs/rapidcheck")
  add_subdirectory(test)
ENDIF()

