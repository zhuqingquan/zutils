cmake_minimum_required(VERSION 3.10)

project(zutils)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(MZ_FORCE_FETCH_LIBS ON)
add_subdirectory(minizip)
set_target_properties(minizip PROPERTIES CXX_STANDARD 17)
include_directories(${CMAKE_BINARY_DIR})

set(SRC_LIST FileUtils.cc TextHelper.cpp)

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    list(APPEND SRC_LIST ProcessHelper.cc)
endif()

add_library(zutils STATIC 
    ${SRC_LIST}
    FilePathHelper.cpp
    ThreadTaskHandler.cpp
    zlog.cpp
    wave-reader.cc
    wave-writer.cc
)
set_target_properties(zutils PROPERTIES CXX_STANDARD 17)

target_include_directories(zutils PUBLIC 
        ${CMAKE_BINARY_DIR}/zutils/
        ${PROJECT_SOURCE_DIR}/minizip
)

add_subdirectory(test zutils_test)