#pragma once

#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <iostream>
//提供一些工具函数

//作为后台程序运行
void daemonize()
{
    pid_t pid;
    int fd;
    if ((pid = fork()) < 0) {
        std::cerr << "[Error] Can't fork!" << std::endl;
    } else if (pid != 0) {
        exit(0);
    }
    //在新的会话运行子进程（脱离原会话组）
    setsid();
    umask(0077);
    if ((fd = open("/dev/null", O_RDWR)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) {
            close(fd);
        }
    }
}