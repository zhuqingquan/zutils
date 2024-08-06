/*
 * @Author: zhuqingquan
 * @Date: 2024-08-05 
 * @FilePath: \zutils\cpp\FileUtils.h
 * @Description: 实现一些文件操作的工具函数，比如解压缩文件
 */
#pragma once
#ifndef _Z_UTILS_FILE_UTILS_H_
#define _Z_UTILS_FILE_UTILS_H_

#include <string>

namespace zutils
{
    int unzipFile(const std::string& zipFilePath, const std::string& destDir);
}//namespace zutils
#endif// _Z_UTILS_FILE_UTILS_H_