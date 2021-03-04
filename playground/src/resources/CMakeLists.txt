add_library(resources FileParser.cpp)
target_include_directories(resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(resources javaLike log exceptions)
