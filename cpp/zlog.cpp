#include "zlog.h"
#include <string>
#if defined(__ANDROID__) || defined(ANDROID)
#include <android/log.h>
#endif

ZLogHandler gHyLogHandler = nullptr;
std::wstring gCurLogFilePath;

/**
 *	这个是默认的写日志的函数，如果用户不调用setHyLogFunc设置写日志的函数，则是用此函数进行写日志
 *	此函数是用的前提是需要先调用zlog_init()进行初始化
 **/
inline void LogFuncHelper(void* userData, int level, const char* fmt, va_list args)
{
#if defined(__ANDROID__) || defined(ANDROID)
	const char* module_name = "sdk"
	switch (level)
	{
	case kLogDebug:
		__android_log_vprint(ANDROID_LOG_DEBUG, module_name, fmt, args);
		break;
	case kLogInfo:
		__android_log_vprint(ANDROID_LOG_INFO, module_name, fmt, args);
		break;
	case kLogWarning:
		__android_log_vprint(ANDROID_LOG_WARN, module_name, fmt, args);
		break;
	case kLogError:
		__android_log_vprint(ANDROID_LOG_ERROR, module_name, fmt, args);
		break;
	case kLogAssert:
		__android_log_vprint(ANDROID_LOG_FATAL, module_name, fmt, args);
		break;
	default:
		break;
	}
#endif
}

ZLogFunc gHyLogFunc = LogFuncHelper;
void* gHyLogFuncUserData = nullptr;

int zlog_init(const wchar_t* logFilePath)
{
    return -1;
}

void zlog_uninit()
{
}

const wchar_t* zlog_getCurLogFilePath()
{
	return gCurLogFilePath.c_str();
}

void zlog_setFunc(ZLogFunc logFunc, void* userData)
{
	gHyLogFunc = logFunc;
	gHyLogFuncUserData = userData;
}

void zlogImpl(int level, const char* format, ...)
{
	if (gHyLogFunc != NULL)
	{
		va_list args;
		va_start(args, format);
		gHyLogFunc(gHyLogFuncUserData, level, format, args);
		va_end(args);
	}
}

ZLogHandler zlog_create(const wchar_t* logFilePath, int* errCode /*= NULL*/)
{
    return nullptr;
}

void zlog_release(ZLogHandler* hlog)
{
}

void zlog_log(ZLogHandler hlog, LogLevel level, const wchar_t* format, ...)
{
}
