#pragma once

// RULE: put as much includes in .cc

#include <arpa/inet.h> // For htons, htonl, etc.
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unordered_set>

#include "server.h"
#include "../parser/http_parser.h"

#include "../thread_pool/thread_pool.h"

class TcpServer : public Server
{
    ThreadPool thread_pool;
    std::thread connection_thread;
    int server_fd;
    struct sockaddr_in address; // sockaddr_in is adapted for ipv4, ipv6 packets.., sockaddr is for general use
    std::vector<struct kevent> events_list;
    std::unordered_set<int> current_fds;
    struct kevent server_monitor;
    int kqueue_instance;

public:
    TcpServer(int nb_threads);
    void start() override;
    void stop() override;
    void create_socket();
    void bind_socket();
    void listen_socket();
    void remove_client(int client_fd);
    void handler_clients();
    ~TcpServer() override = default;
    
};