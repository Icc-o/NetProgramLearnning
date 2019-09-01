#include "Scoket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <errno.h>

Socket::Socket():m_fd(socket(AF_INET, SOCK_STREAM, 0))
{

}

Socket::Socket(int fd):m_fd(fd)
{

}

Socket::~Socket()
{
    close(m_fd);
}

void Socket::init()
{

}

int Socket::getFd()const
{
    return m_fd;
}

bool Socket::bind(NetAddress address)
{
    int ret = ::bind(m_fd,address.getSockaddr(),sizeof(*address.getSockaddr()));
    return (ret==-1)?false:true;
}

bool Socket::connect(NetAddress address)
{
    int ret = ::connect(m_fd, address.getSockaddr(),sizeof(*address.getSockaddr()));
    return (ret==-1)?false:true;
}

bool Socket::listen(int backlog)
{
    int ret = ::listen(m_fd,backlog);
    return (ret==-1)?false:true;
}

Socket Socket::accept(NetAddress* clientAddress)
{
    socklen_t len = sizeof(clientAddress->getSockaddr());
    int ret = ::accept(m_fd, clientAddress->getSockaddr(), &len);
    return Socket(ret);
}

int Socket::send(std::string msg)
{
    //char send_buffer[4096];
    //strcpy(send_buffer, msg.c_str());
    int ret = ::send(m_fd,msg.c_str(),strlen(msg.c_str()), 0);
    std::cout<<"[send] msg:"<<msg<<" size: "<<sizeof(msg.c_str())<<" ret:"<<ret<<std::endl;
    return ret;
}

int Socket::recv(std::string& msg)
{
    char recvMsg[4096];
    memset(recvMsg, 0, sizeof(recvMsg));
    int size = ::recv(m_fd, recvMsg, sizeof(recvMsg), 0);
    if(size<0)
        return size;
    msg = recvMsg;
    return size;
}

bool Socket::getTcpInfo(struct tcp_info* info)
{
    int ret =::getsockopt(m_fd,SOL_TCP,TCP_INFO,info, nullptr);
    return (ret == -1)?false:true;
}

void Socket::shutDown()
{
    ::shutdown(m_fd,SHUT_RDWR);
}

void Socket::setNoDelay(bool tag)
{
    int t_isNodelay = tag?1:0;
    setsockopt(m_fd,SOL_TCP, TCP_NODELAY, &t_isNodelay, sizeof(int));
}


void Socket::setReuseAddress(bool tag)
{
    int t_reuseAddress = tag?1:0;
    setsockopt(m_fd,SOL_SOCKET, SO_REUSEADDR, &t_reuseAddress, sizeof(t_reuseAddress));
}

void Socket::setReusePort(bool tag)
{
    int t_reusePort = tag?1:0;
    setsockopt(m_fd,SOL_SOCKET, SO_REUSEPORT, &t_reusePort, sizeof(t_reusePort));
}

void Socket::setKeepAlive(bool tag)
{

}

void Socket::setSendBufferSize(int size)
{

}

void Socket::setRecvBufferSize(int size)
{

}