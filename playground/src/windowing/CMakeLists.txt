add_library(windowing INTERFACE)
target_include_directories(windowing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

# library link dependencies
target_link_libraries(windowing INTERFACE core ${SDL2_LIBRARY})
