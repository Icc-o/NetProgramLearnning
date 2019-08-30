 cmake_minimum_required(VERSION 3.5)
#CMake 最低版本要求
#cmake_minium_required (VERSION 3.5)

#项目信息，项目名称
project(Client1)

#使用c++11
add_definitions(-std=c++11)

#指定生成目标
add_executable(client_01 client.cpp)
add_executable(server_01 server.cpp)