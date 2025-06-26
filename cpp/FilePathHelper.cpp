#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif
#include "FilePathHelper.h"
#include "TextHelper.h"

using namespace std;

namespace zutils
{

#ifdef _WIN32
wstring getAppPath()
{
	wchar_t buffer[512]={0};
	GetModuleFileName(NULL, buffer ,sizeof(buffer));
	wstring path = wstring(buffer);
	path.erase(path.find_last_of(L"\\"));
	return path;
}

wstring getAppDataPath()
{
	TCHAR szPath[MAX_PATH]; 
	ZeroMemory(szPath, MAX_PATH); 
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	return szPath;
}

wstring getTempPath()
{
	TCHAR szPath[MAX_PATH]; 
	ZeroMemory(szPath, MAX_PATH); 
	GetTempPath(MAX_PATH, szPath);
	return szPath;
}

wstring combine(wstring path)
{
	return combine(getAppPath(), path);
}
#endif

wstring combine(wstring path1, wstring path2)
{
	return path1 + L"\\" + path2;
}


wstring changeExtension(wstring path, wstring extension)
{
	wstring ret = path;
	ret.erase(ret.find_last_of(L".") + 1);
	ret += extension;
	return ret;
}

wstring getPathRoot(wstring path)
{
	if(path == L"")
		return L"";
	wstring ret = path;
	ret = ret.substr(0, ret.find_first_of(L"\\"));
	return ret;
}

wstring getFullPath(wstring path)
{
	if(path == L"")
		return L"";
	wstring ret = path;
	ret = ret.substr(0, ret.find_last_of(L"\\"));
	return ret;
}

wstring getDirectoryName(wstring path)
{
	if(path == L"")
		return L"";
	vector<wstring> items = zutils::split(path, L"\\");
	return items[items.size()-2];
}

wstring getFileName(wstring path)
{
	if(path == L"")
		return L"";
	vector<wstring> items = zutils::split(path, L"\\");
	return items[items.size()-1];
}

string getFileName(const string& path)
{
	if(path.empty())
		return "";
	vector<string> items = zutils::split(path, "\\");
	return items[items.size()-1];
}

wstring getFileNameFromUrl(const wstring& path)
{
	if(path == L"")
		return L"";
	vector<wstring> items = zutils::split(path, L"/");
	return items[items.size()-1];
}

wstring getFileNameWithoutExtension(wstring path)
{
	if(path == L"")
		return L"";
	wstring name = getFileName(path);
	name = name.erase(name.find_last_of(L"."));
	return name;
}

wstring getFileExtension(wstring path)
{
	if(path == L"")
		return L"";
	wstring name = getFileName(path);
	vector<wstring> items = zutils::split(name, L".");
	return items[items.size()-1];
}

string getFileExtension(const string& path)
{
	if(path.empty())
		return "";
	string name = getFileName(path);
	vector<string> items = zutils::split(name, ".");
	return items[items.size()-1];
}
}