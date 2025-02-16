#include "thread_pool/worker.h"
#include <unistd.h>
#include "logging/trace.h"
#include "task/read_request.h"
#include "task/send_response.h"

extern bool isRunning;

Worker::Worker() : events_list(events_size)
{
    kqueue_instance = kqueue();
    if (kqueue_instance < 0)
    {
        TRACE_ERROR("Kqueue creation failed for thread %s", std::this_thread::get_id);
        exit(EXIT_FAILURE);
    }
}
void Worker::operator()()
{
    while (isRunning)
    {
        struct timespec timeout;
        memset(&timeout, 0, sizeof(timeout));
        timeout.tv_nsec = 0;
        timeout.tv_sec = 0;
        // we need timeout, in case we add new client, time it out to be taken into consideration
        int res = kevent(kqueue_instance, nullptr, 0, events_list.data(), 1000, &timeout);
        if (res == -1)
        {
            TRACE_ERROR("An error occured with kevent");
        }
        if (res > 0)
        {
            for (struct kevent event : events_list)
            {
                if (event.flags & EV_ERROR)
                {
                    TRACE_ERROR("Error on client: %d", event.ident);
                    remove_client(event.ident);
                    continue;
                }
                if (event.filter == EVFILT_READ)
                {
                    if (event.flags & EV_EOF)
                    {
                        remove_client(event.ident);
                    }
                    else
                    {
                        ReadRequest{static_cast<int>(event.ident), current_fds, kqueue_instance}();
                    }
                }
            }
        }
    }
}

void Worker::remove_client(int client_fd)
{
    if (current_fds.find(client_fd) == current_fds.end())
        return;
    TRACE_INFO("Remove client: %d", client_fd);
    close(client_fd);
    current_fds.erase(client_fd);
}

void Worker::add_client(int client_fd)
{
    struct kevent client_monitor;
    EV_SET(&client_monitor, client_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    kevent(kqueue_instance, &client_monitor, 1, nullptr, 0, nullptr); // direct add */
    current_fds.insert(client_fd);
}

Worker::~Worker()
{
    for (int client_fd : current_fds)
        {
            close(client_fd);
            TRACE_DEBUG("Closed client %d", client_fd);
        }
        current_fds.clear();
}
