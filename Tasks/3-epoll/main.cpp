#include "event.h"
#include "funcs.h"
#include <iostream>
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "thread_pool/thread_pool.h"

#include "http/Ihttp.h"

#define MAX_EVENTS 100

void test_func1()
{

}

void test_func2(int param)
{
    std::cout<<"thread execute test_func2 ing..."<<std::endl;
    std::cout<<"thread id: "<<std::this_thread::get_id()<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(param));
    std::cout<<"test_func2 execute finished!"<<std::endl;
}

void thread_pool_test()
{

}


void http_test_func()
{
    std::string test_str = "aaa,bbb,ccc,ddd,eee,fff";
    std::string test_str2 = "aaa1bbb1ccc1ddd1eee";
    std::vector<std::string> splited_str = split_str(test_str,',');
    for(auto &s : splited_str)
        std::cout<<"\t"<<s;
    std::cout<<std::endl;
}


//读入的时候开启一个线程调这一段
void read_call_back(Socket s, event &server_event)
{
    std::string recv_msg;
    int ret = s.recv(recv_msg); //recv(triggered_events[i].data.fd,recv_msg, sizeof(recv_msg), 0);
    if(ret < 0)
    {
        std::cerr<<"[recv] return value <0"<<std::endl;
        exit(1);
    }
    else if(ret == 0)
    {
        s.shutDown();
        //在线程池不好把event去掉
        std::cout<<"thread id: "<<std::this_thread::get_id()<<" solving fd: "<<s.getFd()<<std::endl;
        server_event.event_remove(s);
    }
    else
    {
        //从客户端读取数据
        CHttpRequest req;
        req.load_content(recv_msg);
        req.parse_content();
    }

}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr<<"[usage] app [port]"<<std::endl;
        return -1;
    }


    //-------------------部分测试功能代码-------------------------------------
    http_test_func();  //测试分割函数

    //测试线程池
    ThreadPool executor(5);
    //std::future<void> f2 = executor.enqueue(test_func2,2);
    //std::cout<<"thread_pool end"<<std::endl;
    //std::cout<<"thread pool get ret value f2 "<<f2.get()<<std::endl;
    //---------------------------------------------------------------------


    // daemonize();

    signal(SIGPIPE, SIG_IGN);

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
                server_event.event_add(s.getFd(), EPOLLIN|EPOLLET|EPOLLOUT);
                //s.send("[message] Hello from server");
            }
            else
            {
                if(triggered_events[i].events & EPOLLIN)
                {
                    Socket s(triggered_events[i].data.fd);
                    //放到线程池里面去处理了
                    executor.enqueue(read_call_back, s, server_event);
                }
                else if(triggered_events[i].events & EPOLLOUT)
                {
                    // Socket s(triggered_events[i].data.fd);
                    //s.send("HTTP/1.1 200 OK\nServer: loong\nContent-type: text/html\n<html><div><h1>hello world</h1></div></html\n");
                    std::string response_str = "HTTP/1.1 200 OK\nServer: loong\nContent-type: text/html\n<html><div><h1>hello world</h1></div></html\n";
                    // ::send(triggered_events[i].data.fd,response_str.c_str,strlen(response_str.c_str()),0);
                }
            }

            
        }
    }
}
