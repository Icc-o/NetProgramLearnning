#include "NetAddress.h"
#include <string.h>
#include <arpa/inet.h>

NetAddress::NetAddress(std::string ip, short port)
{
    memset(&m_address, 0, sizeof(m_address));
    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(port);
    if(ip.empty())
    {
        m_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        inet_pton(AF_INET,ip.c_str(),&m_address.sin_addr);
    }
}

std::string NetAddress::ip()const
{
    return "";
}

std::string NetAddress::port()const
{
    return std::to_string(m_address.sin_port);
}

sockaddr* NetAddress::getSockaddr()
{
    return (sockaddr*)&m_address;
}
