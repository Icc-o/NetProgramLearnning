#include "event.h"
#include "funcs.h"
#include <iostream>
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr<<"[usage] app [port]"<<std::endl;
        return -1;
    }

    // daemonize();

    //初始化服务器套接字
    NetAddress bind_address(std::string(), atoi(argv[1]));
    Socket server;
    server.bind(bind_address);
    server.setNonBlocking(true);
    server.setReuseAddress(true);
    server.listen(100);
    
    //初始化event
    event server_event(101);
    // struct epoll_event ev;
    // ev.events = EPOLLIN;
    // ev.data.fd = server.getFd();
    server_event.event_add(server, EPOLLIN);

    //进入主循环
    int nfd=0;
    struct epoll_event triggered_events[1000];
    for(;;)
    {
        nfd = epoll_wait(server_event.get_fd(),triggered_events, 1000, -1);
        if(nfd == -1)
        {
            std::cerr<<"[epoll_wait] error"<<strerror(errno)<<std::endl;
            exit(1);
        }
        //遍历处罚的事件
        for(int i=0; i<nfd; i++)
        {
            std::cout<<"triggered fd: "<<triggered_events[i].data.fd<<std::endl;
            if(triggered_events[i].data.fd == server.getFd())
            {
                NetAddress t_add(std::string(), 0);
                Socket s = server.accept(&t_add);
                s.setNonBlocking(true);
                server_event.event_add(s, EPOLLIN|EPOLLET);
            }
            else
            {
                char recv_msg[4096];
                memset(recv_msg, 0, sizeof(recv_msg));
                int ret = recv(triggered_events[i].data.fd,recv_msg, sizeof(recv_msg), 0);
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
                }
                
            }
            
        }
    }
}