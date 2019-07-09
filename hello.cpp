#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static const char MESSAGE[] = "Hello World!\n";
static const int PORT = 9995;

static void listener_cb(struct evconnlistener*, evutil_socket_t, struct sockaddr *, int socklen, void*);
static void conn_writecb(struct bufferevent *, void*);
static void conn_eventcb(struct bufferevent *, short, void*);
static void signale_cb(evutil_socket_t, short, void*);

int main(int argc, char** argv)
{}
