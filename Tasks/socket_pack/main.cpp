#include "Scoket.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <errno.h>
#include "funcs.h"

// app server port 
// app client servr_ip server_port
int main(int argc, char** argv)
{
    if(strcmp(argv[1],"server")==0 && argc==4)
    {
        //run client
        Socket client;
        NetAddress serverAddress(std::string(argv[2]), atoi(argv[3]));
        client.connect(serverAddress);
        std::string send_buffer;
        while(1)
        {
            //直接cin的时候cin把字符按照空格分成几段
            //getline(std::cin,&send_buffer);
            if(strcmp(send_buffer.c_str(), "exit") == 0)
            {
                break;
            }
            client.send(send_buffer);
        }
    }
    else if(strcmp(argv[1],"server")==0 && argc==3)
    {
        daemonize();
        //run server
        Socket server;
        NetAddress bindAddress(std::string(), atoi(argv[2]));
        if(!server.bind(bindAddress))
        {
            std::cout<<"[bind] "<<strerror(errno)<<std::endl;
            return -1; 
        }
        server.listen(10);
        server.setReuseAddress(true);
        NetAddress accept_address("",1);
        Socket client_fd = server.accept(&accept_address);
        if(client_fd.getFd()<0)
        {
            std::cout<<"[accept] failed!"<<" "<<strerror(errno)<<std::endl;
            return -1;
        }
        std::string recv_string;
        while(1)
        {
            int size = client_fd.recv(recv_string);
            if(size > 0)
            {
                std::cout<<"[recv] "<<recv_string<<std::endl;
            }
            if(strcmp(recv_string.c_str(), "exit")==0)
            {
                break;
            }
            client_fd.send(recv_string);
        }
    }
    else 
    {
        std::cout<<"Usage:"<<"app client [server_ip] [server_port]\napp server [port]"<<std::endl;
        return -1;
    }
    return 0;
}