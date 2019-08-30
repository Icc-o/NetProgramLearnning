#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
using namespace std;

int main(int argc, char** argv)
{
    //初始化socket
    int socket_server = socket(AF_INET,SOCK_STREAM, 0);

    //绑定监听地址
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[1]));
    socklen_t socklen = sizeof(address);

    
    int ret = bind(socket_server,(sockaddr*)(&address),socklen);
    if(ret == -1)
    {
        std::cout<<"[error] bind failed! "<<strerror(errno)<<std::endl;
        return -1;
    }

    //调用close之后不会经历TIME_WAIT
    int reuse = 1;
    setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, (char*)(&reuse),sizeof(reuse));
    
    //监听
    ret = listen(socket_server,10);
    if(ret==-1)
    {
        std::cout<<"[error] listen failed!"<<strerror(errno)<<std::endl;
        return -1;
    }

    //获取队列中的第一个连接
    int socket_client = accept(socket_server,nullptr,nullptr);
    if(socket_client<0)
    {
        cout<<"[accept] failed! "<<strerror(errno)<<std::endl;
        return -1;
    }
    //读取数据
    char read_buff[4096];
    while(1)
    {
        memset(read_buff, 0, sizeof(read_buff));
        int size = recv(socket_client,read_buff,sizeof(read_buff),0);
        if(size>0)
        {
            cout<<"[recv data] "<<read_buff<<std::endl;
            if(strcmp(read_buff,"exit") == 0)
            break;
        }
        else
        {
            cout<<"[recv] failed!"<<endl;
            return -1;
        }
        //原样返回
    send(socket_client,read_buff,size,0);
    }

    close(socket_client);
    close(socket_server);

    return 0;
}