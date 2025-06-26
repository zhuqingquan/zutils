#pragma once
#ifndef _Z_UTILS_LOG_H_
#define _Z_UTILS_LOG_H_

#include <stdlib.h>
#include <cstdarg>

//#ifdef WIN32
//
//#ifdef BUILD_ZLOG
//#define ZLOG_API __declspec(dllexport)
//#else
//#define ZLOG_API __declspec(dllimport)
//#endif
//
//#else

#define ZLOG_API

//#endif

#define ZLOG_ERROR_NONE				0		// 调用成功
#define ZLOG_ERROR_FAILED				-1		// 调用失败，可能时参数问题
#define ZLOG_ERROR_ALREADY_INITED		-2		// 日志模块已经初始化过，不可重复初始化
#define ZLOG_ERROR_INIT_FAILED			-3		// 初始化失败，可能是创建或者打开日志文件失败等

enum LogLevel
{
	kLogTrace = 0,
	kLogDebug,
	kLogInfo,
	kLogWarning,
	kLogError,
	kLogAssert,
};

typedef void* ZLogHandler;

extern "C"
{
	/**
	 * @name	zlog_init
	 * @brief	Init log module.
	 *			Create a global ZLogHandler obj, use 'logi' 'logd' 'LOGE' 'LOGWARING'... function to write message to this global ZLogHandler
	 * @return	ZLOG_ERROR_NONE -- success, other -- failed @ref ZLOG_ERROR_FAILED
	 * @usage
	 *			zlog_init("path\xxx.log")
	 *			logi("test log msg %d", i);
	 **/
	ZLOG_API int zlog_init(const wchar_t* logFilePath);
	/**
	 * @name	zlog_uninit
	 * @brief	Uninit huya log module.Release the global ZLogHandler created by zlog_init()
	 **/
	ZLOG_API void zlog_uninit();
	/**
	 * @name	zlog_getCurLogFilePath
	 * @brief	Get current log file path after call zlog_init("path\xxx.log");
	 * @return	Current log file path. return "" if not call zlog_init() yet
	 **/
	ZLOG_API const wchar_t* zlog_getCurLogFilePath();

	// log callback function
	typedef void(*ZLogFunc)(void* userData, int level, const char* fmt, va_list args);
	/**
	 * @name	zlog_setFunc
	 * @brief	Set a user defined log callback function.
	 *			After call this function, the log message write by call 'logi' 'logd' 'LOGE' 'LOGWARING'...
				will pass to user by this callback function.
				Once call zlog_setFunc(), the zlog_init() is useless. The log message will not write to log file any more.
	 **/
	ZLOG_API void zlog_setFunc(ZLogFunc logFunc, void* userData);

	/**
	 * @name	zlog_create
	 * @brief	Create a ZLogHandler for a log file.
	 *			Use in the scenario that need to create and write log message to different file.
	 * @return	ZLogHandler logger object handler. NULL for failed.
	 **/
	ZLOG_API ZLogHandler zlog_create(const wchar_t* logFilePath, int* errCode = NULL);
	/**
	 * @name	zlog_release
	 * @brief	Release the ZLogHandler obj that created by zlog_create()
	 **/
	ZLOG_API void zlog_release(ZLogHandler* hlog);
	/**
	 * @name	zlog_log
	 * @brief	Write message to log file associated with ZLogHandlerhlog.
	 *			Event user called zlog_setFunc(), the log message still write to the log file associated with ZLogHandler hlog.
	 **/
	ZLOG_API void zlog_log(ZLogHandler hlog, LogLevel level, const wchar_t* format, ...);
}

void zlogImpl(int level, const char* format, ...);

#if defined(_WIN32) || defined(WIN32)
// 此处level的值来源于LogLevel
#define logger(level, format, ...)			 do { zlogImpl(level, format, ##__VA_ARGS__); } while(0)

#define  logd(format, ...)  logger(kLogDebug, format, ##__VA_ARGS__)
#define	 logi(format, ...)  logger(kLogInfo, format, ##__VA_ARGS__)
#define  logt(format, ...)  logger(kLogInfo, format, ##__VA_ARGS__)
#define  logw(format, ...)  logger(kLogWarning, format, ##__VA_ARGS__)
#define  loge(format, ...)  logger(kLogError, format, ##__VA_ARGS__)

#define  LOGD(format,...)  logd(format,##__VA_ARGS__)
#define  LOGI(format,...)  logi(format,##__VA_ARGS__)
#define  LOGE(format,...)  logt(format,##__VA_ARGS__)
#define  LOGW(format,...)  logw(format,##__VA_ARGS__)
#define  LOGT(format,...)  loge(format,##__VA_ARGS__)

#define LOGINFO(format,...)		logi(format,##__VA_ARGS__)
#define LOGWARING(format,...)	logw(format,##__VA_ARGS__)
#define LOGERROR(format,...)	loge(format,##__VA_ARGS__)
#define LOGFATAL(format,...)	loge(format,##__VA_ARGS__)
#define LOGDEBUG(format,...)	logd(format,##__VA_ARGS__)

#define log_if_false(condition, format, ...)  do { if(!(condition)) zlogImpl(kLogInfo, format, ##__VA_ARGS__); } while(0)

#else

#define logger(level, format, args...)			 do { zlogImpl(level, format, ##args); } while(0)

#define  logd(format, args...)  logger(kLogDebug, format, ##args)
#define	 logi(format, args...)  logger(kLogInfo, format, ##args)
#define  logt(format, args...)  logger(kLogInfo, format, ##args)
#define  logw(format, args...)  logger(kLogWarning, format, ##args)
#define  loge(format, args...)  logger(kLogError, format, ##args)

#define  LOGD(format,args...)  logd(format,##args)
#define  LOGI(format,args...)  logi(format,##args)
#define  LOGE(format,args...)  logt(format,##args)
#define  LOGW(format,args...)  logw(format,##args)
#define  LOGT(format,args...)  loge(format,##args)

#endif

#endif// _Z_UTILS_LOG_H_
