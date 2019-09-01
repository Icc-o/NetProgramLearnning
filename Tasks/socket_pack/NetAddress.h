#pragma once 
#include <netinet/in.h>
#include <string>

//把sockaddress_in也封装一下
class NetAddress
{
public:
    explicit NetAddress(std::string ip, short port);
    //获取ip
    std::string ip()const;
    //获取端口
    std::string port()const;
    //在部分socket函数中使用
    sockaddr* getSockaddr();

private:
    sockaddr_in m_address;
};