cmake_minimum_required(VERSION 3.10)

project(zutils_test)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest-src)

add_executable(zutils_test 
                testArgParse.cc
                FileUtils_unittest.cc
            )

target_include_directories(zutils_test PUBLIC ../ googletest-src/googletest/include)
target_link_libraries(zutils_test PUBLIC zutils gtest_main minizip)

file(COPY ${PROJECT_SOURCE_DIR}/test.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})