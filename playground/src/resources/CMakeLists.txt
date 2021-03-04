add_library(resources FileParser.cpp)
target_include_directories(resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(src/javaLike)
include_directories(src/exceptions)
include_directories(src/log)

# library link dependencies
target_link_libraries(resources PUBLIC javaLike log exceptions ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
