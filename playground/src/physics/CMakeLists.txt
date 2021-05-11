add_library(physics INTERFACE)
target_include_directories(physics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(physics INTERFACE math)
