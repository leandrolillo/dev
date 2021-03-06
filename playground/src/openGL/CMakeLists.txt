add_library(openGL INTERFACE)
target_include_directories(openGL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

# library link dependencies
target_link_libraries(openGL INTERFACE core windowing video ${OPENGL_LIBRARIES})
