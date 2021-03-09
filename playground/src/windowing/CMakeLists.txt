add_library(windowing INTERFACE)
target_include_directories(windowing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#this needs to be present in every CmakeList that requires files from playground/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

#set(CMAKE_FIND_DEBUG_MODE 1)

#SDL2
find_package(SDL2 REQUIRED)
target_include_directories(windowing INTERFACE ${SDL2_INCLUDE_DIR})
target_link_libraries(windowing INTERFACE ${SDL2_LIBRARY})

# library link dependencies
target_link_libraries(windowing INTERFACE core ${SDL2_LIBRARY})
