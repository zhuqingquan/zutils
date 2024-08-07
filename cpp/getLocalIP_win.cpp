/*
 * @Author: https://www.cnblogs.com/lzpong/p/6137652.html
 * @Date: 2024-04-30 14:58:43
 * @FilePath: \zutils\cpp\getLocalIP_win.cpp
 * @Description: 获取本机所有的IP windows实现
 */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib ")  //linking to the library
using namespace std;
int get_ip()
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    struct sockaddr_in6 *sinp6;
    PHOSTENT hostinfo;
    char hostname[255] = {0}; //主机名
    char *port = "3294";      //端口号
    const char *addr;
    int ilRc;
    gethostname(hostname, sizeof(hostname));
    if((hostinfo = gethostbyname(hostname)) == NULL) //获得本地ipv4地址
    {
        errno = GetLastError();
        fprintf(stderr,"gethostbyname Error:%d\n", errno);
        return 1;
    }
    LPCSTR ip;
    while(*(hostinfo->h_addr_list) != NULL) //输出ipv4地址
    {
        ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
        printf("ipv4 name=%s addr = %s\n\n", hostinfo->h_name, ip);
        hostinfo->h_addr_list++;
    }
    hint.ai_family = AF_INET6;        //hint 的限定设置
    hint.ai_socktype = SOCK_STREAM;   //这里可是设置 socket type    比如  SOCK_DGRAM
    hint.ai_flags = AI_PASSIVE;       // flags 的标志很多。常用的有AI_CANONNAME;
    hint.ai_protocol = 0;             //设置协议  一般为0，默认
    hint.ai_addrlen = 0;              //下面不可以设置，为0，或者为NULL
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    ilRc = getaddrinfo(hostname, port, &hint, &ailist); //通过主机名获得地址信息
    if (ilRc < 0)
    {
        char str_error[100];
        strcpy(str_error, (char *)gai_strerror(errno));
        printf("str_error = %s", str_error);
        return 0;
    }
    if(ailist == NULL)
    {
        printf("sorry not find the IP address,please try again \n");
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) //显示获取的信息
    {
        aip->ai_family == AF_INET6;
        sinp6 = (struct sockaddr_in6 *)aip->ai_addr;    //为什么是for 循环 ，先向下看
        int i;
        printf("ipv6 addr = ");
        for(i = 0; i < 16; i++)
        {
            if(((i-1)%2) && (i>0))
            {
                printf(":");
            }
            printf("%02x",sinp6->sin6_addr.u.Byte[i]);
        }
        printf(" \n");
        printf(" \n");
    }
}
int main(){

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );//initiate the ws2_32.dll and match the version
    if ( err != 0 )
    {
        return 0;
    }
    if ( LOBYTE( wsaData.wVersion ) != 1 ||   //if the version is not matched ,then quit and terminate the ws3_32.dll
        HIBYTE( wsaData.wVersion ) != 1 )
    {
        WSACleanup( );
        return 0;
    }
    get_ip();
    WSACleanup( );
    return 0;
}