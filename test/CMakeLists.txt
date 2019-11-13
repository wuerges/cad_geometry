add_executable(TestBase test.cxx)
target_link_libraries(TestBase
    cadgeometry
    rapidcheck
  )

add_executable(TestRTree test_rtree.cxx)
target_link_libraries(TestRTree
    cadgeometry
    rapidcheck
  )
