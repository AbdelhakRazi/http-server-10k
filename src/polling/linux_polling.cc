#include "polling/polling.h"
#include "linux_polling.h"
#include <sys/epoll.h>
#include "logging/trace.h"


extern bool isRunning;
int LinuxPolling::create_queue()
{
    return epoll_create1(0);
}

int LinuxPolling::add_user(int queue_instance, int user)
{
    struct epoll_event monitor;
    monitor.events = EPOLLIN | EPOLLET;
    monitor.data.fd = user;
    return epoll_ctl(queue_instance, EPOLL_CTL_ADD, user, &monitor);
}

void LinuxPolling::wait_server_events(int queue_instance,
                             std::function<void()> onSuccess,
                             std::function<void()> onError)
{
        struct epoll_event event;
        while (isRunning)
        {

            int res = epoll_wait(queue_instance, 
                                &event, 1, 
                                -1);
            if (res == -1)
            {
                onError();
            }
            else if (res > 0)
            {
                onSuccess();
            }
        }
}

void LinuxPolling::wait_worker_events(int queue_instance,
    int timeout,
    std::function<void(int)> onSuccess,
    std::function<void(int)> onError)
{
        struct epoll_event events[MAX_SIZE];
        while (isRunning)
        {
            int res = epoll_wait(queue_instance,
                             events, MAX_SIZE,
                             timeout);
            if (res == -1)
            {
                TRACE_ERROR("An error occured with epoll_wait");
            }
            else if (res > 0)
            {
                for (int i = 0; i < res; i++)
                {
                    struct epoll_event event = events[i];
                    if (event.events & EPOLLERR)
                    {
                        TRACE_ERROR("Error on client: %d", event.data.fd);
                        onError(event.data.fd);
                        continue;
                    }
                    if (event.events & EPOLLHUP)
                        {
                            onError(event.data.fd);
                            continue;
                        }
                    if (event.events & EPOLLIN)
                    {
                            onSuccess(event.data.fd);
                    }
                }
            }
        }
}
