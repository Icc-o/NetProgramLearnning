#include "event.h"
#include "funcs.h"
#include <iostream>
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EVENTS 100

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr<<"[usage] app [port]"<<std::endl;
        return -1;
    }

    daemonize();

    std::cout<<"[argv1]"<<argv[1]<<std::endl;
    //初始化服务器套接字
    Socket server;
    NetAddress bind_address(std::string(), atoi(argv[1]));
    if(!server.bind(bind_address))
    {
        std::cout<<"[bind] error!"<<strerror(errno)<<std::endl;
        exit(-1);
    }
    server.setNonBlocking(true);
    server.setReuseAddress(true);
    server.listen(10);

    //初始化event
    event server_event(100);
    server_event.event_add(server.getFd(),EPOLLIN);

    //进入主循环
    int nfd=0;
     epoll_event triggered_events[MAX_EVENTS];
    for(;;)
    {
        nfd = epoll_wait(server_event.get_fd(),triggered_events, MAX_EVENTS, -1);
        if(nfd == -1)
        {
            std::cerr<<"[epoll_wait] error"<<strerror(errno)<<std::endl;
            exit(1);
        }
        std::cout<<"[nfd]"<<nfd<<std::endl;
        //遍历处罚的事件
        for(int i=0; i<nfd; i++)
        {
            std::cout<<"triggered fd: "<<triggered_events[i].data.fd<<std::endl;
            if(triggered_events[i].data.fd == server.getFd())
            {
                NetAddress t_add(std::string(), 0);
                Socket s = server.accept(&t_add);
                std::cout<<"get connect fd"<<s.getFd()<<std::endl;
                s.setNonBlocking(true);
                server_event.event_add(s.getFd(), EPOLLIN|EPOLLET);
                s.send("[message] Hello from server");
            }
            else
            {
                Socket s(triggered_events[i].data.fd);

                std::string recv_msg;
                int ret = s.recv(recv_msg); //recv(triggered_events[i].data.fd,recv_msg, sizeof(recv_msg), 0);
                if(ret < 0)
                {
                    std::cerr<<"[recv] return value <0"<<std::endl;
                    exit(1);
                }
                else if(ret == 0)
                {
                    std::cout<<"[server] client "<<triggered_events[i].data.fd<<" disconnect from servre"<<std::endl;
                    close(triggered_events[i].data.fd);
                    server_event.event_remove(triggered_events[i].data.fd);
                }
                else
                {
                    //从客户端读取数据
                    std::cout<<"[recv] msg: "<<recv_msg<<std::endl;
                    if(recv_msg == "exit")
                        exit(-1);
                }
                
            }
            
        }
    }
}
