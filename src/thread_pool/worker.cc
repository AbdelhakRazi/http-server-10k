#include "thread_pool/worker.h"
#include <unistd.h>
#include "logging/trace.h"
#include "task/read_request.h"
#include "task/send_response.h"
#include "polling/polling_factory.h"
#include "polling/event_type.h"

extern bool isRunning;

Worker::Worker()
{
    polling = PollingFactory::createPolling();
    kqueue_instance = polling->create_queue();
    if (kqueue_instance < 0)
    {
        TRACE_ERROR("Kqueue creation failed for thread %s", std::this_thread::get_id);
        exit(EXIT_FAILURE);
    }
}
void Worker::operator()()
{
    polling->wait_events(kqueue_instance, 0, EventType::WORKER, 
        [&]() {
            ReadRequest{static_cast<int>(event.ident), current_fds, kqueue_instance}();
        },
        [&]() {
            remove_client(event.ident);
        }
    )
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
