clHelper的用法
clHelper是从OpenCLHelper项目拷贝的。用法如下：
```cmake
#To use this library, do roughly the following in your CMakeLists.txt
    # 除了SET，也可以使用cmake -DclHelper_DIR=<path to clHelper>
    SET(clHelper_DIR <path to this clHelper directory>)
     
    # this defines all the helper macros
    INCLUDE(${clHelper_DIR}/clHelper.cmake)

    # this builds the clHelper library (optional, if you want
    # to manually access the globally generated symbols)
    ADD_SUBDIRECTORY(${clHelper_DIR})

    # 当你需要构建cl语言成为kernels
    # 则可能需要使用OPENCL_INCLUDE_DIRECTORIES、OPENCL_ADD_DEFINITIONS、COMPILE_OPENCL
    # specify include paths for #include's in opencl files:
    OPENCL_INCLUDE_DIRECTORIES(mySearchPath/subdir1 mySearchPath/subdir2)

    # specify global defines for opencl files
    OPENCL_ADD_DEFINITIONS(-DMY_DEFINE=32)

    # compile some opencl kernels. This properly preprocessor-exapands
    # and test-compiles the given .cl files (once to asm, once to llvm),
    # and puts the preprocessor-expanded code (as a char[] array)
    # into dedicated .c files (that can be accessed through the
    # implicit EMBEDDED_OPENCL_KERNELS variable
    COMPILE_OPENCL(myFile1.cl myFile2.cl)

    # 头文件路径
    include_directories(${clHelper_DIR})
    # build a library/executable that embeds these kernels
    ADD_LIBRARY(myLib myFile1.c myFile2.c ${EMBEDDED_OPENCL_KERNELS})
    TARGET_LINK_LIBRARIES(myLib clHelperLib)
```
大部分代码直接从以下开源项目中拷贝：
[OpenCLHelper](https://github.com/ingowald/OpenCLHelper.git)
[oclutils](https://github.com/nbigaouette/oclutils.git)