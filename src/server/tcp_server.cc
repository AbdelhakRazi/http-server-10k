#include "tcp_server.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <sys/resource.h>

#include "task/read_request.h"
#include "task/send_response.h"
#include "trace/trace.h"
#include "polling/polling_factory.h"
#include "polling/polling.h"


bool isRunning{true};

namespace server
{
    TcpServer::TcpServer(int nb_threads) : thread_pool{nb_threads}
    {
        // initialize address set
        memset(&address, 0, sizeof(address));
        address.sin_port = htons(port_number); // convert to network endian format.
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_family = AF_INET;
        // problem we had with accept: os had 256 file descriptors limits. let's push it to 65535 and retry
        struct rlimit rlim;
        rlim.rlim_cur = rlim.rlim_max = max_fd;
        if (setrlimit(RLIMIT_NOFILE, &rlim) < 0)
        {
            TRACE_ERROR("Failed to increase server file descriptors, can't handle 10k clients");
        };
        polling = PollingFactory::createPolling();
    }

    void TcpServer::start()
    {
        create_socket();
        bind_socket();
        listen_socket();
    }

    void TcpServer::stop()
    {
        isRunning = false;
        if (server_fd != -1)
        {
            close(server_fd);
            TRACE_DEBUG("Closed Server socket"); // preprocessor doesn't treat macros as part of namespace
        }
    }

    void TcpServer::create_socket()
    {
        if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        {
            TRACE_ERROR("cannot create socket");
            exit(EXIT_FAILURE);
        }
        // asynchronous socket.
        // Set SO_REUSEADDR
        int reuse_addr = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1)
        {
            TRACE_ERROR("Failed to set SO_REUSEADDR");
            close(server_fd);
        }

        // Set SO_REUSEPORT (Linux and macOS support this)
        int reuse_port = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port)) == -1)
        {
            TRACE_ERROR("Failed to set SO_REUSEPORT");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        int flags = fcntl(server_fd, F_GETFL, 0);
        if (flags == -1 || fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            TRACE_ERROR("fcntl failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        TRACE_DEBUG("Serever fd %d", server_fd);
    }

    void TcpServer::bind_socket()
    {
        if (bind(server_fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0)
        {
            TRACE_ERROR("cannot bind");
            exit(EXIT_FAILURE);
        };
    }

    void TcpServer::listen_socket()
    {
        if (listen(server_fd, backlog_size) < 0)
        {
            TRACE_ERROR("listen failed");
            exit(EXIT_FAILURE);
        };
        int server_kqueue = polling->create_queue();
        if (server_kqueue == -1)
        {
            TRACE_ERROR("Kqueue creation failed");
            exit(EXIT_FAILURE);
        }
        // first error: no need to set up server for monitoring, add thread that will monitor it continuously.
        polling->add_user(server_kqueue, server_fd);
        TRACE_INFO("Server listening on %d", server_fd);
        polling->wait_server_events(server_kqueue, [&]()
                             { accept_client(); }, []()
                             {
                    TRACE_DEBUG("Error accepting new client");
                    exit(EXIT_FAILURE); });
    }
    void TcpServer::accept_client()
    {
        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t addr_len;
        client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_addr), &addr_len);
        TRACE_DEBUG("Client added %d", client_fd);
        if (client_fd > 0)
        {
            thread_pool.add_client(client_fd);
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            else
            {
                TRACE_ERROR("Accept failed");
            }
        }
    }
    TcpServer::~TcpServer()
    {
    }
} // namespace server