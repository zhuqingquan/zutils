/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2025-02-20 19:02:27
 * @LastEditors: zhuqingquan zqq_222@163.com
 * @LastEditTime: 2025-02-20 20:37:43
 * @FilePath: /zutils/cpp/test/testOpencl.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <CL/cl.h>
#include "OclUtils.hpp"

int main(int argc, char* argv[])
{
    printf("start...\n");

    OpenCL_platforms_list platlist;
    platlist.Initialize(OPENCL_PLATFORMS_NVIDIA);
    platlist.Print();
    platlist.Print_Preferred();

    OpenCL_device dev;
    auto& ctx = dev.Get_Context();
    
    OpenCL_runtime clrt;
    
    return 0;
}