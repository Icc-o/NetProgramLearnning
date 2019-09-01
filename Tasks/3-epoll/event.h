#pragma once 
#include <stdint.h>
#include "socket_base/Scoket.h"
//把epoll封装一下，提供一个event类

struct epoll_event;

class event
{
public:
    explicit event(int max_events = 30);
    ~event();

    //添加事件
    void event_add(int fd, uint32_t event);
    void event_add(Socket s, uint32_t event);
    
    //删除事件
    void event_remove(int fd);
    void event_remove(Socket s);
    
    //修改事件
    void event_modify(int fd, uint32_t event);
    void event_modify(Socket s, uint32_t event);

    //
    void event_ctrl(uint32_t opt, int fd, epoll_event* ev);

    int get_fd()const{return m_epoll_fd;}

private:
    int m_epoll_fd;
};