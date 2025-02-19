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

int MacPolling::wait_events(int queue_instance, PollingParameters polling_parameters)
{
    return kevent(queue_instance, 
        nullptr, 0, 
        polling_parameters.events, polling_parameters.max_events, 
        polling_parameters.timeout);
    return 0;
}
