add_library(resources INTERFACE)
target_include_directories(resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/adapters)

# library link dependencies
target_link_libraries(resources INTERFACE javaLike log exceptions)
