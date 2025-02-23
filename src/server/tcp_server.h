#ifndef __TCP_SERVER__H__
#define __TCP_SERVER__H__

#include <arpa/inet.h> // For htons, htonl, etc.
#include <unordered_set>
#include <memory>
#include "server/server.h"
#include "thread_pool/thread_pool.h"

class Polling;
namespace server {
class TcpServer : public Server
{
    ThreadPool thread_pool;
    int server_fd;
    struct sockaddr_in address; // sockaddr_in is adapted for ipv4, ipv6 packets.., sockaddr is for general use
    std::unique_ptr<Polling> polling;
    static constexpr int events_size = 1024;
    static constexpr int backlog_size = 1024;
    static constexpr int max_fd = 10000;
    static constexpr int port_number = 8081;

public:
    explicit TcpServer(int nb_threads); // one argument constructor
    void start() override;
    void stop() override;
    void create_socket();
    void bind_socket();
    void listen_socket();
    void accept_client();
    ~TcpServer() override;
    
};
} // namespace server
#endif  //!__TCP_SERVER__H__
// RULE: put as much includes in .cc

