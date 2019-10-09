cmake_minimum_required(VERSION 3.0)
project(cad_geometry CXX)
set (CMAKE_CXX_STANDARD 17)

add_library(libs
  src/base.cxx  
  )
include_directories(include)

add_subdirectory("ext-libs/rapidcheck")

add_subdirectory(test)

