#include "mac_polling.h"
#include <sys/event.h>

int MacPolling::create_queue()
{
    return kqueue();
}

int MacPolling::add_user(int queue_instance, int user)
{
    struct kevent monitor;
    EV_SET(&monitor, user, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    return kevent(queue_instance, 
        &monitor, 1, 
        nullptr, 0, 
        nullptr); // direct add */
}

int MacPolling::wait_events(int queue_instance, PollingEvent& events, int timeout)
{
    struct timespec ktimeout;
    if(timeout != -1) {
        memset(&timeout, 0, sizeof(ktimeout));
        ktimeout.tvsec = ktimeout / 1000;
    }
    struct kevent kevent;
    return kevent(queue_instance, 
        nullptr, 0, 
        &kevent, 1, 
        &ktimeout);
    
}
