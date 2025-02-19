#ifndef __MAC_PAREMETERS__H__
#define __MAC_PAREMETERS__H__

#ifdef __APPLE__
#include <vector>
#include <sys/event.h>
#include <sys/types.h>

struct MacParameters {
    struct kevent* events;
    struct timespec* timeout;
    int max_events;
};

#endif

#endif  //!__MAC_PAREMETERS__H__