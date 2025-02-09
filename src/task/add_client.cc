#include "task/add_client.h"

#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sys/event.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <thread>

#include "http/http_request.h"
#include "parser/http_parser.h"
#include "logging/trace.h"

extern bool isRunning;
void AddClient::operator() ()
{
    struct kevent server_monitor;
    struct kevent server_event;
    int server_kqueue = kqueue();
    EV_SET(&server_monitor, server_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    kevent(server_kqueue, &server_monitor, 1, nullptr, 0, nullptr); // direct add */
    while (isRunning)
    {
        // problem: kevent won't detect signals since it is in subthread
        // temp solution: use timeout
        struct timespec timeout;
        memset(&timeout, 0, sizeof(timeout));
        timeout.tv_sec = 10;
        int res = kevent(server_kqueue, nullptr, 0, &server_event, 1, &timeout);
        if (res > 0)
        {
            accept_client();
        }
        else if (res == -1) {
            if(errno == EINVAL) {
                TRACE_DEBUG("Invalid timeout filter");
            }
            TRACE_DEBUG("Error accepting new client");
            exit(EXIT_FAILURE);
        }
    }
}

void AddClient::accept_client()
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
    if (client_fd > 0)
    {
        TRACE_INFO("Client %d connected", client_fd);
        struct kevent client_monitor;
        EV_SET(&client_monitor, client_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
        kevent(kqueue_instance, &client_monitor, 1, nullptr, 0, nullptr); // direct add */
        current_fds.insert(client_fd);
    }
    else
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        else
        {
            TRACE_ERROR("Accept failed");
        }
    }
}
