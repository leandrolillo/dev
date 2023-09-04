add_definitions(-w) # hide warnings

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # not necessary, but encouraged

project (playground)

file(MAKE_DIRECTORY target)

set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

add_subdirectory(src/core)
add_subdirectory(src/exceptions)
add_subdirectory(src/javaLike)
add_subdirectory(src/log)
add_subdirectory(src/math)
add_subdirectory(src/resources)
add_subdirectory(src/video)
add_subdirectory(src/openGL)
add_subdirectory(src/parser)
add_subdirectory(src/audio)
add_subdirectory(src/geometry)
add_subdirectory(src/physics)
add_subdirectory(src/test)

add_executable(playground src/Main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

target_link_libraries(playground physics core math openGL audio test)