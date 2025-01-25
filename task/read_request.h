#ifndef __READ_DATA__H__
#define __READ_DATA__H__

#include <unordered_set>
#include <vector>
// TODO: add copy constructor and move constructor, we need it when popping the element from the queue
class ReadRequest
{
    int client_fd;
    std::unordered_set<int> current_fds;
    int kqueue_instance;

public:
    ReadRequest(int client_fd,
             std::unordered_set<int> &current_fds,
             int kqueue_instance) : client_fd{client_fd},
                                    current_fds{current_fds},
                                    kqueue_instance{kqueue_instance} {}
    ReadRequest(const ReadRequest &other) : client_fd{other.client_fd},
                                      current_fds{other.current_fds},
                                      kqueue_instance{other.kqueue_instance} {}
    ReadRequest(const ReadRequest &&other) : client_fd{other.client_fd},
                                 current_fds{std::move(other.current_fds)},
                                 kqueue_instance{other.kqueue_instance} {}
    void operator() ();
    ~ReadRequest() = default;
};
#endif //!__READ_DATA__H__