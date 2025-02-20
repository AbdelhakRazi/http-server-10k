#include "polling/mac_polling.h"

#include <sys/event.h>
#include <sys/types.h>
#include <string.h>
#include "logging/trace.h"

extern bool isRunning;
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

void MacPolling::wait_events(int queue_instance,
                             int timeout,
                             EventType event_type,
                             std::function<void()> onSuccess,
                             std::function<void()> onError)
{
    if (event_type == EventType::SERVER)
    {
        struct kevent event;
        while (isRunning)
        {

            int res = kevent(queue_instance,
                             nullptr, 0,
                             &event, 1,
                             nullptr);
            if (res == -1)
            {
                onError();
            }
            else if (res == 0)
            {
                onSuccess();
            }
        }
    }
    else if (event_type == EventType::WORKER)
    {
        struct timespec ktimeout;
        if (timeout != -1)
        {
            memset(&timeout, 0, sizeof(ktimeout));
            ktimeout.tv_sec = timeout / 1000;
        }
        struct kevent events[MAX_SIZE];
        while (isRunning)
        {
            int res = kevent(queue_instance,
                             nullptr, 0,
                             events, MAX_SIZE,
                             &ktimeout);
            if (res == -1)
            {
                TRACE_ERROR("An error occured with kevent");
            }
            else if (res > 0)
            {
                for (int i = 0; i < res; i++)
                {
                    struct kevent event = events[i];
                    if (event.flags & EV_ERROR)
                    {
                        TRACE_ERROR("Error on client: %d", event.ident);
                        onError();
                        continue;
                    }
                    if (event.filter == EVFILT_READ)
                    {
                        if (event.flags & EV_EOF)
                        {
                            onError();
                        }
                        else
                        {
                            onSuccess();
                        }
                    }
                }
            }
        }
    }
}
