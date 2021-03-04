add_library(windowing INTERFACE)
target_include_directories(windowing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(windowing INTERFACE core ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
