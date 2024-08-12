/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2024-08-12 
 * @FilePath: /zutils/cpp/libtext.h
 * @Description: some helper function for text
 */
#include <string>
#include <vector>
#include <map>

namespace zutils
{
	using std::string;
	using std::wstring;
	using std::vector;
	using std::map;

	vector<wstring> split(const wstring &str, const wstring &delimiter);
	wstring format(wstring fmt, ...);
	string format(string fmt, ...);

	wstring Int64ToWString(__int64 v);
	__int64 wstringToInt64(const wstring& str);
	wstring IntToWString(int v);
	int wstringToInt(const wstring& str);
	wstring UIntToWString(unsigned int v);
	//将wstring转换为unsinged int,此方法会将字符串"12345ddd"转换为整数12345，此时可使用下一个wstringToUInt方法
	unsigned int wstringToUInt(const wstring& str);
	//将wstring转换为unsinged int，此方法转换字符串"12345ddd"时将返回失败false
	bool wstringToUInt(const wstring& str, unsigned int* outResult);
	wstring DoubleToWString(double v);
	double wstringToDouble(wstring str);

	wstring string2wstring(const string &str);
	string wstring2string(const wstring &wstr);
	string UTF8_To_string(const string & str);
	string string_To_UTF8(const string & str);
	wstring UTF8_To_wstring(const string & str);
	string wstring_To_UTF8(const wstring & str);

	bool contains(const wstring& str, const wstring& substr);
	bool startWith(const wstring& str, const wstring& with);
	bool endWith(const wstring& str, const wstring& with);
	wstring trim(const wstring& str);
	wstring trimLeft(const wstring& str, const wstring& trimstr);
	wstring trimRight(const wstring& str, const wstring& trimstr);

	wstring toUpper(wstring str);
	wstring toLower(wstring str);

	wstring replace(const wstring& str, const wstring& src, const wstring& dest);

	wstring startWString(const wstring& str, const int num);
	wstring endWString(const wstring& str, const int num);

	wstring readFromFile(wstring path);
	wstring readFromFileAsSingleLine(wstring path);
	void writeToFile(wstring str, wstring path);

	map<wstring, wstring> readKeyValuesFromFile(wstring path);
	void writeKeyValuesToFile(wstring path, map<wstring, wstring> dict);

	std::string UrlEncode(const std::string& str);
	std::string UrlDecode(const std::string& szToDecode);


	//解析rtsp的url，将url中的用户名和密码提取出来，并生成一个新的不包含用户名密码的url
	bool rtsp_url_password(const string& url, string& name, string& password, string& newUrl);

	//GUID与STR转换
	BOOL str2Guid(const std::wstring& strGUID, GUID& Guid);
	std::wstring guid2Str(GUID *guid);
}