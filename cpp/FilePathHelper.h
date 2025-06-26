/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2023-05-12
 * @FilePath: \zBaseMediaFiltersLib\utils\inc\FilePathHelper.h
 * @Description: 一些用于辅助进行路径、文件名称等操作的函数
 */
#include <string>

namespace zutils
{
	using std::wstring;
	using std::string;

#ifdef _WIN32
	wstring getAppPath();
	wstring getAppDataPath();
	wstring getTempPath();
	wstring combine(wstring path);
#endif

	wstring combine(wstring path1, wstring path2);

	wstring changeExtension(wstring path, wstring extension);
	wstring getPathRoot(wstring path);
	wstring getFullPath(wstring path);
	wstring getDirectoryName(wstring);
	wstring getFileName(wstring path);
	string getFileName(const string& path);
	wstring getFileNameFromUrl(const wstring& path);
	wstring getFileNameWithoutExtension(wstring path);
	wstring getFileExtension(wstring path);
	string getFileExtension(const string& path);
}