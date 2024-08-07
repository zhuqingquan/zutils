/*
 * @Author: zhuqingquan
 * @Date: 2024-08-07 
 * @FilePath: \zutils\cpp\http.h
 * @Description: implement http request base on libcurl
 */
#pragma once
#ifndef _UTILS_HTTP_HELPER_H_
#define _UTILS_HTTP_HELPER_H_

#include <string>

typedef int ResultCode;         // 结果码

#define RC_OK 0                 // success
#define RC_INIT_FAILED -1       // init lib failed
#define RC_REQUEST_FAILED -2       // init lib failed

class HttpRequest
{
public:
    HttpRequest(const std::string& url);
    ResultCode post(const std::string& data, std::string& rsp);
    ResultCode get(std::string& rsp);

private:
    void* m_curl = nullptr;
    std::string m_url;
};

#endif//_UTILS_HTTP_HELPER_H_