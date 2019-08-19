cmake_minimum_required (VERSION 2.6)
project (playground)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

add_executable(playground src/Main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(OPENGL REQUIRED)

message(STATUS "")
message(STATUS "OpenGL:" ${OPENGL_INCLUDE_DIR} " " ${OPENGL_LIBRARIES})
message(STATUS "")

include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
target_link_libraries(playground ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
