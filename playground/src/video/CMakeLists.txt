add_library(video INTERFACE)
target_include_directories(video INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/adapters)

#this needs to be present in every CmakeList that requires files from playground/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# LibJpeg
#find_package(JPEG REQUIRED) # could not make this f****g library work -> incompatibilities between detected header and library files
#target_link_libraries(video INTERFACE ${JPEG_LIBRARY})
#target_include_directories(video INTERFACE ${JPEG_INCLUDE_DIR})

# LibJpeg
find_package(PNG REQUIRED)
target_link_libraries(video INTERFACE ${PNG_LIBRARY})
target_include_directories(video INTERFACE ${PNG_INCLUDE_DIR})

# SDL2_IMAGE
find_package(SDL2_IMAGE REQUIRED)
target_link_libraries(video INTERFACE ${SDL2_IMAGE_LIBRARY})
target_include_directories(video INTERFACE ${SDL2_IMAGE_INCLUDE_DIR})

# library link dependencies
target_link_libraries(video INTERFACE resources math)
target_include_directories(video INTERFACE)
