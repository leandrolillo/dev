add_library(geometry Geometry.cpp intersection/IntersectionTester.cpp)

target_include_directories(geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/intersection)

# library link dependencies
target_link_libraries(geometry PUBLIC math)
