cmake_minimum_required(VERSION 3.5)

project(App)

add_definitions(-std=c++11)

#列出当前文件夹下的所有文件
aux_source_directory(. DIR_SRCS)

add_subdirectory(socket_base)

add_executable(app1 ${DIR_SRCS})

target_link_libraries(app1 socket)