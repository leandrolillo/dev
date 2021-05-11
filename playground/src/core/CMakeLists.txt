add_library(core Playground.cpp)

# make sure this library headers are included
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(core PUBLIC javaLike log resources)
