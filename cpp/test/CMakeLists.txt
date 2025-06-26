cmake_minimum_required(VERSION 3.10)

project(zutils_test)

# 先fetch googletest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
#add_subdirectory(googletest-src)

add_executable(zutils_test 
                testArgParse.cc
                FileUtils_unittest.cc
            )

#target_include_directories(zutils_test PUBLIC ../ googletest-src/googletest/include)
target_link_libraries(zutils_test PUBLIC zutils gtest_main minizip)

file(COPY ${PROJECT_SOURCE_DIR}/test.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
include(GoogleTest)
gtest_discover_tests(zutils_test)