#ifndef __LINUX_PARAMETERS__H__
#define __LINUX_PARAMETERS__H__

#ifdef __linux__
#include <sys/epoll.h>

#define MAX_EVENTS 128
struct LinuxParemeters {
    struct epoll_event events[MAX_EVENTS];
    struct epoll_event event;
};
#endif

#endif  //!__LINUX_PARAMETERS__H__