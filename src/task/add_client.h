#ifndef __ADD_CLIENT__H__
#define __ADD_CLIENT__H__

#include <unordered_set>
#include <vector>
// TODO: add copy constructor and move constructor, we need it when popping the element from the queue
class AddClient
{
    int server_fd;
    int kqueue_instance;
    std::unordered_set<int> &current_fds;

public:
    AddClient(int server_fd,
              int kqueue_instance,
              std::unordered_set<int> &current_fds) : server_fd{server_fd},
                                                      current_fds{current_fds},
                                                      kqueue_instance{kqueue_instance} {}
    AddClient(const AddClient &other) : server_fd{other.server_fd},
                                        current_fds{other.current_fds},
                                        kqueue_instance{other.kqueue_instance} {}
    AddClient(AddClient &&other) : server_fd{other.server_fd},
                                   current_fds{other.current_fds},
                                   kqueue_instance{other.kqueue_instance} {}
    void operator() ();
    void accept_client();
    ~AddClient() = default;
};
#endif //!__ADD_CLIENT__H__
