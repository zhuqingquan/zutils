// TextHelper.cpp : Defines the exported functions for the DLL application.
//

#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#endif
#include <algorithm>  
#include <functional>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <codecvt>
#include <locale>
#include <stdarg.h>
#include "TextHelper.h"

using namespace std;

namespace zutils
{

vector<wstring> split(const wstring &str, const wstring &delimiter)
{
	vector<wstring> ret;
	size_t last = 0;
	size_t index = str.find_first_of(delimiter,last);
	while ( index != string::npos )
	{
		ret.push_back(str.substr(last,index-last));
		last = index + 1;
		index = str.find_first_of(delimiter,last);
	}
	if ( index - last > 0 )
	{
		ret.push_back(str.substr(last ,index - last));
	}
	return ret;
}

vector<string> split(const string &str, const string &delimiter)
{
	vector<string> ret;
	size_t last = 0;
	size_t index = str.find_first_of(delimiter,last);
	while ( index != string::npos )
	{
		ret.push_back(str.substr(last,index-last));
		last = index + 1;
		index = str.find_first_of(delimiter,last);
	}
	if ( index - last > 0 )
	{
		ret.push_back(str.substr(last ,index - last));
	}
	return ret;
}

wstring format(wstring fmt, ...)
{
	wchar_t buffer[4096] = {0};

	va_list valist;
	va_start(valist, fmt);
#ifdef _WINDOWS
	_vsnwprintf(buffer, sizeof(buffer)/sizeof(wchar_t), fmt.c_str(), valist);
#else
	vswprintf(buffer, sizeof(buffer)/sizeof(wchar_t), fmt.c_str(), valist);
#endif
	va_end(valist);

	return wstring(buffer);
}

string format(string fmt, ...)
{
	char buffer[4096] = {0};

	va_list valist;
	va_start(valist, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt.c_str(), valist);
	va_end(valist);

	return string(buffer);
}

wstring Int64ToWString(int64_t v)
{
	wchar_t buf[128] = {0};
	swprintf(buf, 128, L"%lld", v);
	return std::move(wstring(buf));
}

int64_t wstringToInt64(const wstring& str)
{
	int64_t v = 0;
	swscanf(str.c_str(), L"%lld", &v);
	return v;
}

wstring IntToWString(int v)
{
	wchar_t buf[128] = {0};
	swprintf(buf, 128, L"%d", v);
	return wstring(buf);
}

wstring UIntToWString(unsigned int v)
{
	wchar_t buf[128] = {0};
	swprintf(buf, 128, L"%u", v);
	return wstring(buf);
}

unsigned int wstringToUInt(const wstring& str)
{
	unsigned int v = 0;
	swscanf(str.c_str(), L"%u", &v);
	return v;
}

bool wstringToUInt( const wstring& str, unsigned int* outResult )
{
	if(str.size()>=1024)
	{
		return false;
	}
	unsigned int v = 0;
	wchar_t tail[1024] = {0};
	int varc = swscanf(str.c_str(), L"%u%1024s", &v, tail);
	if(varc==1)
	{
		*outResult = v;
		return true;
	}
	return false;
}

wstring DoubleToWString(double v)
{
	wchar_t buf[128] = {0};
	swprintf(buf, 128, L"%lf", v);
	return wstring(buf);
}

double wstringToDouble(wstring str)
{
	double v = 0;
	swscanf(str.c_str(), L"%lf", &v);
	return v;
}

int wstringToInt(const wstring& str)
{
	int v = 0;
	swscanf(str.c_str(), L"%d", &v);
	return v;
}

wstring string2wstring(const string &str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}

string wstring2string(const wstring &wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

#ifdef _WINDOWS
string UTF8_To_string(const string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	string retStr = pBuf;

	delete []pBuf;
	delete []pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
} 

string string_To_UTF8(const string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	string retStr(pBuf);

	delete []pwBuf;
	delete []pBuf;

	pwBuf = NULL;
	pBuf  = NULL;

	return retStr;
} 

string wstring_To_UTF8(const wstring & str)
{
	int nwLen = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char * pBuf = new char[nwLen + 1];
	ZeroMemory(pBuf, nwLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), nwLen, pBuf, nwLen, NULL, NULL);

	string retStr(pBuf);
	delete []pBuf;
	pBuf  = NULL;
	return retStr;
} 

wstring UTF8_To_wstring(const string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	wstring retStr = pwBuf;

	delete []pwBuf;

	pwBuf = NULL;

	return retStr;
} 
#else
string UTF8_To_string(const string & str) { return str; }
string string_To_UTF8(const string & str) { return str; }
string wstring_To_UTF8(const wstring & str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(str);
}
wstring UTF8_To_wstring(const string & str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}
#endif

bool contains(const wstring& str, const wstring& substr)
{
	std::size_t found = str.find(substr);
	if (found==std::string::npos)
		return false;
	else
		return true;
}

bool startWith(const wstring& str, const wstring& with)
{
	return str.compare(0, with.size(), with)==0 ? true : false; 
}

bool endWith(const wstring& str, const wstring& with)
{
	if(str.size()<with.size())
		return false;
	return str.compare(str.size()-with.size(), with.size(), with)==0 ? true : false;  ;
}

wstring trim(const wstring& str)
{
	wstring ret = str;
	ret.erase(0, ret.find_first_not_of(' '));
	ret.erase(ret.find_last_not_of(' ')+1);
	return ret;
}

wstring trimLeft(const wstring& str, const wstring& trimstr)
{
	wstring ret = str;
	ret.erase(0, ret.find_first_not_of(trimstr.c_str()));
	return ret;
}

wstring trimRight(const wstring& str, const wstring& trimstr)
{
	wstring ret = str;
	ret.erase(ret.find_last_not_of(trimstr.c_str())+1);
	return ret;
}

wstring toUpper(wstring str)
{
	transform(str.begin(),str.end(),str.begin(),::toupper);
	return str;
}

wstring toLower(wstring str)
{
	transform(str.begin(),str.end(),str.begin(),::tolower);
	return str;
}

wstring replace(const wstring& str, const wstring& search, const wstring& replace)
{
	wstring subject = str;
	size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;

}

wstring startWString(const wstring& str, const int num)
{
	wstring tmp = L"";
	if ((int)str.length() > num)
	{
		tmp.append(str.begin(), str.begin() + num);
	}
	else
	{
		tmp.append(str.begin(), str.end());
	}
	return tmp;
}

wstring endWString(const wstring& str, const int num)
{
	wstring tmp = L"";
	if ((int)str.length() > num)
	{
		tmp.append(str.end() - num, str.end());
	}
	else
	{
		tmp.append(str.begin(), str.end());
	}
	return tmp;
}

wstring readFromFile(wstring path)
{
	string str;
#ifdef _WINDOWS
	ifstream fs(path.c_str());
#else
	ifstream fs(zutils::wstring2string(path).c_str());
#endif
	int a = fs.get();
	int b = fs.get();
	int c = fs.get();
	if(a==0xEF && b==0xBB && c==0xBF)
	{
		str = std::string((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
	}
	else
	{
		fs.seekg(0);
		str = std::string((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
	}
	wstring ret = zutils::UTF8_To_wstring(str);
	return ret;
}

wstring readFromFileAsSingleLine(wstring path)
{
	wstring text = readFromFile(path);
	text = replace(text, L"\n", L", ");
	text = trim(text);
	return text;
}

void writeToFile(wstring str, wstring path)
{
#ifdef _WINDOWS
	ofstream fs(path.c_str());
#else
	ofstream fs(zutils::wstring2string(path).c_str());
#endif
	unsigned char bom[] = { 0xEF,0xBB,0xBF };
    fs.write((char*)bom, sizeof(bom));
	string text = zutils::wstring_To_UTF8(str);
	fs.write(text.c_str(), text.size());
	fs.close();
}

map<wstring, wstring> readKeyValuesFromFile(wstring path)
{
	map<wstring, wstring> dict;
	wstring text = readFromFile(path);
	vector<wstring> lines = split(text, L"\n");
	for(size_t i=0; i<lines.size(); i++)
	{
		wstring line = lines[i];
		vector<wstring> items = split(line, L"=");
		if(items.size()>=2)
		{
			wstring key = trim(items[0]);
			wstring value = trim(items[1]);
			dict[key] = value;
		}
	}
	return dict;
}

void writeKeyValuesToFile(wstring path, map<wstring, wstring> dict)
{
	wstring text = L"";
	for(map<wstring, wstring>::iterator i=dict.begin(); i!=dict.end(); i++)
	{
		text += format(L"%s = %s\n", i->first.c_str(), i->second.c_str());
	}
	writeToFile(text, path);
}

std::string UrlDecode(const std::string& szToDecode)
{  
	std::string result;  
	int hex = 0;  
	for (size_t i = 0; i < szToDecode.length(); ++i)  
	{  
		switch (szToDecode[i])  
		{  
		case '+':  
			result += ' ';  
			break;  
		case '%':  
			if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))  
			{
				std::string hexStr = szToDecode.substr(i + 1, 2);  
				hex = strtol(hexStr.c_str(), 0, 16);
				//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@] 

				//可以不经过编码直接用于URL  
				if (!((hex >= 48 && hex <= 57) || //0-9  
					(hex >=97 && hex <= 122) ||   //a-z  
					(hex >=65 && hex <= 90) ||    //A-Z  
					//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
					hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29 
					|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f 
					|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f 
					))  
				{  
					result += char(hex);
					i += 2;  
				}  
				else result += '%';  
			}else {  
				result += '%';  
			}  
			break;  
		default:  
			result += szToDecode[i];  
			break;  
		} 

	}  
	return zutils::UTF8_To_string(result);  
} 

string UrlEncode(const string& str)
{
	//按照要求，必须先转换为utf8，再urlencode
	string utfstr = zutils::string_To_UTF8(str);

	static char hex[] = "0123456789ABCDEF";  
	string dst;  

	for (size_t i = 0; i < utfstr.size() && utfstr[i] != 0; i++)  
	{  
		unsigned char ch = utfstr[i];  
		if (isalnum(ch) || ch == '.')  
		{  
			dst += ch;  
		}  
		else
		{  
			unsigned char c = static_cast<unsigned char>(utfstr[i]);  
			dst += '%';  
			dst += hex[c >> 4];  
			dst += hex[c % 16];  
		}  
	}  
	return dst;
}

bool rtsp_url_password(const string& url, string& name, string& password, string& newUrl)
{
	const std::string rtspHeader = "rtsp://";
	std::string::size_type p1 = url.find(rtspHeader);
	if(url.npos==p1)
		return false;
	std::string::size_type p2 = url.find('@', p1);
	if(url.npos==p2)
	{
		//没有用户名密码
		newUrl = url;
		return true;
	}
	//查找rtsp://头之后的第一个'/'，如果'@'的位置位于这个'/'之后，则找到的不是标志用户名密码的'@'
	std::string::size_type p3 = url.find('/', p1+rtspHeader.length());
	if(url.npos!=p3 && p3<p2)
	{
		//查找成功，且'@'的位置位于这个'/'之后
		newUrl = url;
		return true;
	}
	//抽取用户名、密码字符串
	std::string authString = url.substr(p1+rtspHeader.length(), p2-(p1+rtspHeader.length()));
	//拆分用户名、密码
	std::string::size_type p4 = authString.find(':');
	if(p4==authString.npos)
	{
		return false;
	}
	name = authString.substr(0, p4);
	password = authString.substr(p4+1);
	string newUrlTmp = rtspHeader;
	newUrlTmp += url.substr(p2+1);
	newUrl = newUrlTmp;
	return true;
}

#ifdef _WINDOWS
BOOL str2Guid(const std::wstring& szGUID, GUID& outGuid)
{
	if (szGUID.empty())
	{
		outGuid = GUID_NULL;
		return TRUE;
	}
	char buf[128] = {0};
	GUID& Guid = *((GUID*)buf);//{0};
	int nRet = _stscanf(szGUID.data(),
		L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		&Guid.Data1, 
		&Guid.Data2,
		&Guid.Data3,
		&Guid.Data4[0], &Guid.Data4[1],
		&Guid.Data4[2], &Guid.Data4[3], &Guid.Data4[4], &Guid.Data4[5], &Guid.Data4[6], &Guid.Data4[7]
	);

	outGuid = Guid;

	return nRet > 8;
}

std::wstring guid2Str(GUID *guid)
{
	if (guid == NULL)
	{
		return L"";
	}
	wchar_t guid_string[39];
	swprintf(
		guid_string, sizeof(guid_string) / sizeof(guid_string[0]),
		L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		guid->Data1, guid->Data2, guid->Data3,
		guid->Data4[0], guid->Data4[1], guid->Data4[2],
		guid->Data4[3], guid->Data4[4], guid->Data4[5],
		guid->Data4[6], guid->Data4[7]);
	return guid_string;  
}
#endif//_WINDOWS
}