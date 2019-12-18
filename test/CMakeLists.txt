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

add_executable(TestFastRTree test_fastrtree.cxx)
target_link_libraries(TestFastRTree
    cadgeometry
    rapidcheck
  )
      
add_executable(TesteRTreeQueue test_rtree_queue.cxx)
target_link_libraries(TesteRTreeQueue
    cadgeometry
    rapidcheck
  )
      