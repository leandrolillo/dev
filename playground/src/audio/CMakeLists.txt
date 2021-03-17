add_library(audio INTERFACE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenAL REQUIRED)

#find_package(libVorbis REQUIRED)
#find_package(libOgg REQUIRED)


# make sure this library headers are included
target_include_directories(audio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENAL_INCLUDE_DIR} ${LIBVORBIS_INCLUDE_DIR} ${LIBOGG_INCLUDE_DIR})

# library link dependencies
target_link_libraries(audio INTERFACE javaLike log resources math ${OPENAL_LIBRARY} ${LIBVORBIS_LIBRARY} ${LIBOGG_LIBRARY})
