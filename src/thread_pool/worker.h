#ifndef __WORKER__H__
#define __WORKER__H__
#include <arpa/inet.h> // For htons, htonl, etc.
#include <unordered_set>
#include <memory>
#include "polling/polling.h"

class Worker
{
    std::unordered_set<int> current_fds;
    std::shared_ptr<Polling> polling; // can't use unique_ptr since we'll copy it into two places
    int kqueue_instance;
    static constexpr int events_size = 1024;

public:
    Worker();
    void remove_client(int client_fd);
    void add_client(int client_fd);
    void operator() ();
    ~Worker();
    
};
#endif  //!__WORKER__H__


// RULE: put as much includes in .cc

