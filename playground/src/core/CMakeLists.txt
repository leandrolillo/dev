add_library(core Playground.cpp)

include_directories(src/javaLike)
include_directories(src/resources)
include_directories(src/log)

# make sure these headers are included
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(core PUBLIC javaLike log resources ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
