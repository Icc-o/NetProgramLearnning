//The first socket demo
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

const short PORT = 8889;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout<<"[Usage] client [server address] [port]"<<std::endl;
        return -1;
    }
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        cout<<"socket failed!"<<endl;
        return -1;
    }

    //announce servers' address
    sockaddr_in serAddr;
    memset(&serAddr,0,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[1]));
    inet_pton(AF_INET,argv[2],&serAddr.sin_addr);
    socklen_t socklen = sizeof(serAddr);
    int result = connect(sockfd,(sockaddr*)(&serAddr),socklen);
    if(result == -1)
        return -1;

    char send_buffer[4096];
    std::cin >> send_buffer;
    while(strcmp(send_buffer, "exit") != 0)
    {    
        char buff[4096];
        send(sockfd,send_buffer,strlen(send_buffer),0);
        int recv_size = recv(sockfd, buff, sizeof(buff), 0);
        if(recv_size >0 )
        {
            std::cout<<"[recv] "<<buff<<endl;
        }
        memset(send_buffer, 0, sizeof(send_buffer));
        std::cin >> send_buffer;
    }
    close(sockfd);
    return 0;
}