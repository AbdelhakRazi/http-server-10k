#include "tcp_server.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include "../task/read_data.h"
#include "../task/send_request.h"
#include "../task/add_client.h"

bool isRunning{true};
std::condition_variable cond;

TcpServer::TcpServer(int nb_threads) : events_list(1024), thread_pool{nb_threads}
{
    // initialize address set
    memset(&address, 0, sizeof(address));
    address.sin_port = htons(8080); // convert to network endian format.
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    // problem we had with accept: os had 256 file descriptors limits. let's push it to 65535 and retry
    struct rlimit rlim;
    rlim.rlim_cur = rlim.rlim_max = 10000;
    if (setrlimit(RLIMIT_NOFILE, &rlim) < 0)
    {
        std::cout << "Failed to increase server file descriptors, can't handle 10k clients" << std::endl;
    };
}

void TcpServer::start()
{
    create_socket();
    bind_socket();
    listen_socket();
    while (isRunning)
    {
        handler_clients();
    }
}
void TcpServer::stop()
{
    isRunning = false;
    cond.notify_all();
    connection_thread.join();
    if (server_fd != -1)
    {
        close(server_fd);
        std::cout << "server socket closed" << std::endl;
        for (int client_fd : current_fds)
        {
            close(client_fd);
            std::cout << "Closed client: " << client_fd << std::endl;
        }
        current_fds.clear();
    }
}
void TcpServer::create_socket()
{
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }
    // asynchronous socket.
    int opt = 1;
    int flags = fcntl(server_fd, F_GETFL, 0);
    if (flags == -1 || fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void TcpServer::bind_socket()
{
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("cannot bind");
        exit(EXIT_FAILURE);
    };
}

void TcpServer::listen_socket()
{
    if (listen(server_fd, 1024) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    };
    kqueue_instance = kqueue();
    if (kqueue_instance == -1)
    {
        perror("Kqueue creation failed");
        exit(EXIT_FAILURE);
    }
    // first error: no need to set up server for monitoring, add thread that will monitor it continuously.
    AddClient addtask{server_fd, kqueue_instance, current_fds};
    connection_thread = std::thread{&AddClient::execute, addtask};
    std::cout << "server fd: " << server_fd << std::endl;
}

void TcpServer::remove_client(int client_fd)
{
    if (current_fds.find(client_fd) == current_fds.end()) return;
    std::cout << "Remove client: " << client_fd << std::endl;
    close(client_fd);
    current_fds.erase(client_fd);
}

void TcpServer::handler_clients()
{
    int res = kevent(kqueue_instance, nullptr, 0, events_list.data(), 1024, nullptr);
    if (res == -1)
    {
        perror("An error occured with kevent");
    }
    if (res > 0)
    {
        for (struct kevent event : events_list)
        {
            if (event.flags & EV_ERROR)
            {
                std::cout << "Error on client: " << event.ident << std::endl;
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
                    thread_pool.add_task(std::make_unique<ReadData>(event.ident, current_fds, kqueue_instance));
                    thread_pool.add_task(std::make_unique<SendResponse>(event.ident));
                }
            }
        }
    }
}