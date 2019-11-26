cmake_minimum_required(VERSION 3.15)
project(cad_geometry CXX)
set (CMAKE_CXX_STANDARD 17)


add_library(cadgeometry)
target_sources(cadgeometry
  PRIVATE
    src/base.cxx
    src/rtree_adapter.cxx
    src/rtree_queue.cxx
    src/rtree_queue_2.cxx
    src/rtree_queue_filter.cxx
  PUBLIC
    # include/geometry.hpp
    # include/rtree_adapter.hpp
    # include/RTree.h
    # include/treap.hpp
)
target_include_directories(cadgeometry
  PUBLIC
    include
)

IF(EXISTS "${PROJECT_SOURCE_DIR}/ext-libs/rapidcheck/CMakeLists.txt")
  add_subdirectory("ext-libs/rapidcheck")
  add_subdirectory(test)
ENDIF()

