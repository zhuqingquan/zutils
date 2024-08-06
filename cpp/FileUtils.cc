/*
 * @Author: zhuqingquan
 * @Date: 2024-08-05 
 * @FilePath: \zutils\cpp\FileUtils.cc
 * @Description: 实现一些文件操作的工具函数，比如解压缩文件
 */
#include "FileUtils.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "minizip/unzip.h"

int zutils::unzipFile(const std::string& zipFilePath, const std::string& destDir)
{
    unzFile zipFile = unzOpen(zipFilePath.c_str());
    if (zipFile == nullptr) {
        //std::cerr << "Cannot open zip file: " << zipFilePath << std::endl;
        return -1;
    }

    if (unzGoToFirstFile(zipFile) != UNZ_OK) {
        //std::cerr << "Cannot go to first file in zip: " << zipFilePath << std::endl;
        unzClose(zipFile);
        return -2;
    }

    do {
        char filename[256];
        unz_file_info fileInfo;
        if (unzGetCurrentFileInfo(zipFile, &fileInfo, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
            //std::cerr << "Cannot get file info in zip: " << zipFilePath << std::endl;
            unzClose(zipFile);
            return -3;
        }

        std::string fullPath = destDir + "/" + filename;
        if (filename[strlen(filename) - 1] == '/') {
            std::filesystem::create_directories(fullPath);
        } else {
            if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                //std::cerr << "Cannot open file in zip: " << filename << std::endl;
                unzClose(zipFile);
                return -4;
            }

            std::ofstream outFile(fullPath, std::ios::binary);
            if (!outFile) {
                //std::cerr << "Cannot create file: " << fullPath << std::endl;
                unzCloseCurrentFile(zipFile);
                unzClose(zipFile);
                return -5;
            }

            std::vector<char> buffer(8192);
            int bytesRead;
            while ((bytesRead = unzReadCurrentFile(zipFile, buffer.data(), buffer.size())) > 0) {
                outFile.write(buffer.data(), bytesRead);
            }

            outFile.close();
            unzCloseCurrentFile(zipFile);
        }
    } while (unzGoToNextFile(zipFile) == UNZ_OK);

    unzClose(zipFile);
    return 0;
}