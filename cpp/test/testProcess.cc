/*
 * @Author: zhuqingquan
 * @Date: 2024-08-04 
 * @FilePath: \zutils\cpp\test\testProcess.cc
 * @Description: test for Process class
 */
#include <gtest/gtest.h>
#include "process.h"
#include <thread>
#include "FileUtils.h"

TEST(Process, create)
{
    zutils::Process process;
    int ret = process.create("C:\\Windows\\system32\\cmd.exe", "", /*CREATE_NEW_CONSOLE*/0);
    std::this_thread::sleep_for(std::chrono::milliseconds(30000));
    EXPECT_EQ(ret, 0);
}

TEST(FileUtils, unzip)
{
    std::string zipFilePath = "D:\\project\\fakeaudiodrivingserver.zip";
    std::string destDir = "E:\\test";
    int ret = zutils::unzipFile(zipFilePath, destDir);
    EXPECT_EQ(ret, 0);
}