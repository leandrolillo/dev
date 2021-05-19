add_library(geometry INTERFACE)

target_include_directories(geometry INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(geometry INTERFACE math)
