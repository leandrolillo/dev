add_library(log INTERFACE)
target_include_directories(log INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(log INTERFACE javaLike)