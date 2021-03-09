add_library(openGL INTERFACE)
target_include_directories(openGL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(openGL INTERFACE ${OPENGL_LIBRARIES})
target_include_directories(openGL INTERFACE ${OPENGL_INCLUDE_DIR})

# library link dependencies
target_link_libraries(openGL INTERFACE core windowing video)
