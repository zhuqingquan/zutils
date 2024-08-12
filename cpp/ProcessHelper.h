/*
 * @Author: zhuqingquan
 * @Date: 2024-08-04 
 * @FilePath: \zutils\cpp\process.h
 * @Description: basic functions for process
 */
#pragma once
#ifndef _Z_UTILS_PROCESS_H_
#define _Z_UTILS_PROCESS_H_

#include <string>
#include <Windows.h>

namespace zutils
{

class Process
{
public:
    Process();
    ~Process();

    int create(const std::string& exe, const std::string& cmdline, int createFlags);
    bool close();

private:
    Process(const Process& robj) = delete;
    Process& operator=(const Process& robj) = delete;

    HANDLE m_process = INVALID_HANDLE_VALUE;
    HANDLE m_mainThread = INVALID_HANDLE_VALUE;
    int m_pid = 0;
    int m_tid = 0;
};//class Process

}//namespace zutils

#endif/ _Z_UTILS_PROCESS_H_