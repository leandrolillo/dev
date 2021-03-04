add_library(math Math3d.cpp)
target_include_directories(math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(math PUBLIC exceptions javaLike ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
