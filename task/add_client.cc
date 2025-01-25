#include "add_client.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sys/event.h>
#include <arpa/inet.h>
#include "../request/http_request.h"
#include "../parser/http_parser.h"
#include <sys/fcntl.h>
#include <thread>

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
        int res = kevent(server_kqueue, nullptr, 0, &server_event, 1, nullptr);
        if (res > 0)
        {
            accept_client();
        }
    }
}

void AddClient::accept_client()
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
    if (client_fd > 0)
    {
        std::cout << "Client added: " << client_fd << std::endl;
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
            std::cout << "Accept failed, errno: " << errno
                      << " (" << strerror(errno) << ")" << std::endl;
        }
    }
}
