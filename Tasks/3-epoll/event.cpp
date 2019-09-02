#include "event.h"
#include <sys/epoll.h>
#include <iostream>

event::event(int max_events):m_epoll_fd(epoll_create(max_events))
{
}

event::~event()
{

}

void event::event_add(int fd, uint32_t event)
{
    if(event == EPOLLIN)
    std::cout<<"[event] add event EPOLLIN"<<std::endl;
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = event;
    this->event_ctrl(EPOLL_CTL_ADD,fd,&ev);
}

void event::event_add(Socket s, uint32_t event)
{
    this->event_add(s.getFd(),event);
}

void event::event_remove(int fd)
{

}

void event::event_remove(Socket s)
{
    this->event_remove(s.getFd());
}

void event::event_modify(int fd, uint32_t event)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = event;
    event_ctrl(EPOLL_CTL_MOD, fd, &ev);
}

void event::event_modify(Socket s, uint32_t event)
{
    this->event_modify(s.getFd(), event);
}

void event::event_ctrl(uint32_t opt, int fd, epoll_event* ev)
{
    int ret = epoll_ctl(m_epoll_fd,opt,fd,ev);
    if(ret == -1)
    std::cout<<"[event] epoll_ctrl failed"<<std::endl;
}