#include "ProcessHelper.h"
#include <Windows.h>
#include "TextHelper.h"
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

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
        if(!TerminateProcess(m_process, 1))
        {
            DWORD err = GetLastError();
        }
        CloseHandle(m_process);
        m_process = INVALID_HANDLE_VALUE;
        m_pid = 0;
    }
    return false;
}

bool zutils::killProcessByName(const std::wstring& processName)
{
    HRESULT hres;

    // Initialize COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        //std::cerr << "Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
        return false;
    }

    // Initialize security
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL);

    if (FAILED(hres))
    {
        //std::cerr << "Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return false;
    }

    // Obtain the initial locator to WMI
    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID *)&pLoc);

    if (FAILED(hres))
    {
        //std::cerr << "Failed to create IWbemLocator object. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return false;
    }

    // Connect to WMI
    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc);

    if (FAILED(hres))
    {
        //std::cerr << "Could not connect to WMI. Error code = 0x" << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres))
    {
        //std::cerr << "Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // Use the IWbemServices pointer to make requests of WMI
    IEnumWbemClassObject* pEnumerator = NULL;
    std::wstring query = L"SELECT ProcessId FROM Win32_Process WHERE Name = '" + processName + L"'";
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        //std::cerr << "WMI query failed. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // Get the data from the query
    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;
        hr = pclsObj->Get(L"ProcessId", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr))
        {
            DWORD pid = vtProp.uintVal;
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 1);
                CloseHandle(hProcess);
            }
        }
        VariantClear(&vtProp);
        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return true;
}

bool zutils::killProcessByPID(DWORD pid)
{
    // Open the process with the specified PID
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL)
    {
        //std::cerr << "Failed to open process with PID " << pid << ". Error: " << GetLastError() << std::endl;
        return false;
    }

    // Terminate the process
    if (!TerminateProcess(hProcess, 1))
    {
        //std::cerr << "Failed to terminate process with PID " << pid << ". Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    // Close the process handle
    CloseHandle(hProcess);
    return true;
}