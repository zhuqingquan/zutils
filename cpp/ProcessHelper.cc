#include "ProcessHelper.h"
#include <Windows.h>
#include "libtext.h"

using namespace zutils;

Process::Process()
{

}

Process::~Process()
{
    this->close();
}

int Process::create(const std::string& exe, const std::string& cmdline, int createFlags)
{
    std::wstring wstrExe = zutils::UTF8_To_wstring(exe);
    std::wstring wstrCmdline = zutils::UTF8_To_wstring(cmdline);
    STARTUPINFO si = {0};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pInfo = {0};
	BOOL b = ::CreateProcess(
		wstrExe.empty() ? NULL : (wchar_t*)wstrExe.c_str(), 
		wstrCmdline.empty() ? NULL : (wchar_t*)wstrCmdline.c_str(), 
		NULL, NULL, NULL, 
		createFlags, 
		NULL, NULL, 
		&si, 
		&pInfo);
    if(!b)
    {
        return GetLastError();
    }
    m_process = pInfo.hProcess;
    m_mainThread = pInfo.hThread;
    m_pid = pInfo.dwProcessId;
    m_tid = pInfo.dwThreadId;
    return 0;
}

bool Process::close()
{
    if(m_mainThread!=INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_mainThread);
        m_mainThread = INVALID_HANDLE_VALUE;
        m_tid = 0;
    }
    if(m_process!=INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_process);
        m_process = INVALID_HANDLE_VALUE;
        m_pid = 0;
    }
    return false;
}