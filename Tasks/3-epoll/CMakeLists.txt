cmake_minimum_required(VERSION 3.5)

project(App)

add_definitions(-std=c++11)

SET(CMAKE_BUILD_TYPE DEBUG)

#link_libraries("/usr/lib")
link_libraries(libpthread.so)

#列出当前文件夹下的所有文件
aux_source_directory(. DIR_SRCS)

add_subdirectory(socket_base)

add_subdirectory(http)

add_executable(app1 ${DIR_SRCS})

target_link_libraries(app1 socket http)
