#include "FileUtils.h"
#include <gtest/gtest.h>

TEST(FileUtilsTest, readFile)
{
    std::string filePath = "test.txt";
    std::string content = zutils::readFile(filePath);
    EXPECT_EQ(content, "hello world");

    std::string exeFilePath = "C:\\Windows\\regedit.exe";
    std::string exeContent = zutils::readFile(exeFilePath);
    EXPECT_FALSE(exeContent.empty());
}