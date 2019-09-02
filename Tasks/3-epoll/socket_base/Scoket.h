#pragma once
#include "NetAddress.h"

class Socket
{
public:
    explicit Socket(int fd);
    explicit Socket();
    ~Socket();

    //在这里初始化套接字
    void init();
    //获取fd
    int getFd();
    //绑定地址
    bool bind(NetAddress address);
    //发起连接
    bool connect(NetAddress address);
    //监听
    bool listen(int backlog);
    //获取连接(顺便返回客户端的地址信息)
    Socket accept(NetAddress* clientAddress);
    //发送
    int send(std::string msg);
    //接收
    int recv(std::string &msg);
    //获取tcp信息
    bool getTcpInfo(struct tcp_info* info);
    //关闭连接
    void shutDown();
    //设置非阻塞
    void setNoDelay(bool tag);
    //设置地址复用（绑定一个已在使用的地址）
    void setReuseAddress(bool tag);
    //设置端口复用（多个进程/线程帮顶一个端口）
    void setReusePort(bool tag);
    //心跳
    void setKeepAlive(bool tag);
    //设置发送缓冲区大小
    void setSendBufferSize(int size);
    //设置接受缓冲区大小
    void setRecvBufferSize(int size);
    //设置非阻塞
    void setNonBlocking(bool tag);
private:
     int m_fd;

};