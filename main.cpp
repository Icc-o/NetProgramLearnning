#include <iostream>
#include "event2/event.h"
using namespace std;


void cb1(int sock, short what, void* arg)
{
    struct event_base *base = arg;
    event_base_loopbreak(base);
}

int main()
{
//    const char** methods = event_get_supported_methods();
//    for(int i=0; methods[i]!=nullptr; i++)
//    {
//        cout<<"aaa: "<<methods[i]<<endl;
//    }

    evutil_socket_t a;
    struct event_base *watchdog_event;
    watchdog_event = event_new(base, watchdog_event);



    cout << "Hello World!" << endl;
    return 0;
}

void cb_func(evutil_socket_t fd, short what, void* arg)
{
    const char* data =arg;
    printf("Got an event on socket %d:%s%s%s%s [%s]",
           (int)fd,
           (what&EV_TIMEOUT)?"timeout":"",
           (what&EV_READ)   ?"read":"",
           (what&EV_WRITE)  ?"write":"",
           (what&EV_SIGNAL) ?"signal":"",
           data);
}

void main_loop(evutil_socket_t fd1, evutil_socket_t fd2)
{
    struct event *ev1, ev2;
    struct timeval five_seconds={5,0};
    struct event_base* base = event_base_new();
    ev1 = event_new(base, fd1, EV_TIMEOUT|EV_READ|EV_PERSIST
                    , cb_func, (char*)"Reading event");
    ev2 = event_new(base, fd2, EV_WRITE|EV_PERSIST,cb_func,
                    (char*)"Writing event");

    event_add(ev1, &five_seconds);
    event_add(ev2, NULL);
    event_base_dispatch(base);
}







