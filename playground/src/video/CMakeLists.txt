add_library(video INTERFACE)
target_include_directories(video INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/adapters)


find_package(PNG REQUIRED)
# include_directories(${PNG_INCLUDE_DIR})

# library link dependencies
target_link_libraries(video INTERFACE resources math ${PNG_LIBRARY})
