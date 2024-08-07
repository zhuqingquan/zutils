cmake_minimum_required(VERSION 3.10)

project(zutils)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(MZ_FORCE_FETCH_LIBS ON)
add_subdirectory(minizip)
set_target_properties(minizip PROPERTIES CXX_STANDARD 17)

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

add_library(zutils STATIC 
            #DevInfo.cc
            FileUtils.cc
            )
set_target_properties(zutils PROPERTIES CXX_STANDARD 17)

target_include_directories(zutils PUBLIC 
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/minizip
)
#add_subdirectory(test zutils_test)