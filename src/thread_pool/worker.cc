#include "thread_pool/worker.h"

#include <unistd.h>
#include <thread>

#include "trace/trace.h"
#include "task/read_request.h"
#include "task/send_response.h"
#include "polling/polling_factory.h"
#include "polling/polling.h"


extern bool isRunning;

Worker::Worker()
{
    polling = std::move(PollingFactory::createPolling());
    kqueue_instance = polling->create_queue();
    if (kqueue_instance < 0)
    {
        TRACE_ERROR("Kqueue creation failed for thread %s", std::this_thread::get_id);
        exit(EXIT_FAILURE);
    }
}
void Worker::operator()()
{
    polling->wait_worker_events(kqueue_instance, 0, 
        [&](auto socket_id) {
            ReadRequest{static_cast<int>(socket_id), current_fds, kqueue_instance}();
        },
        [&](auto socket_id) {
            remove_client(socket_id);
        }
    );
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
    polling->add_user(kqueue_instance, client_fd);
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
