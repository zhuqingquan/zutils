/*
 * @Author: zhuqingquan
 * @Date: 2024-08-07 
 * @FilePath: \zutils\cpp\http.cc
 * @Description: implement http request base on libcurl
 */
#include "http.h"
#include <curl/curl.h>

HttpRequest::HttpRequest(const std::string& url)
{
    CURL* curl = curl_easy_init();
    if(curl!=nullptr)
    {
        m_curl = curl;
        m_url = url;
    }
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), realsize);
    return realsize;
}

ResultCode HttpRequest::post(const std::string& data, std::string& rsp)
{
    if(m_curl==nullptr) return RC_INIT_FAILED;
    // set params
    // set curl header
    struct curl_slist* header_list = NULL;
    // der_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
    header_list = curl_slist_append(header_list, "Content-Type:application/json; charset = UTF-8");
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, header_list);    

    curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &rsp);

    CURLcode res = curl_easy_perform(m_curl);
    return res;
}

ResultCode HttpRequest::get(std::string& rsp)
{
    if(m_curl==nullptr) return RC_INIT_FAILED;
    // Set the URL for the GET request
    curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str());
    
    // Set the write callback function to handle the response data
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &rsp);

    // Perform the GET request
    CURLcode res = curl_easy_perform(m_curl);

    // Check for errors
    if(res != CURLE_OK) {
        return RC_REQUEST_FAILED;
    }

    return RC_OK;
}