#include "thread_pool/thread_pool.h"

#include <iostream>
#include <sys/event.h>

#include "logging/trace.h"
#include <sys/fcntl.h>
#include <unistd.h>

ThreadPool::ThreadPool(int nb_threads)
{
    for (int i = 0; i < nb_threads; i++)
    {
        Worker worker;
        workers.push_back(worker);
        threads.push_back(std::thread(worker));
    }
}
void ThreadPool::add_client(int client_fd)
{
    // add clients in round robin manner.
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags == -1 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        TRACE_ERROR("fcntl failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    int recv_buf_size = 4 * 1024 * 1024; // 4MB
    setsockopt(client_fd, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, sizeof(recv_buf_size));
    workers[current_worker].add_client(client_fd);
    current_worker = (current_worker + 1) % workers.size();
    TRACE_INFO("New client %d", client_fd);
}

// Problem? Destructor not being called
ThreadPool::~ThreadPool()
{
    for (int i = 0; i < nb_threads; i++)
    {
        threads[i].join();
    }
    TRACE_DEBUG("All threads ended");
}
