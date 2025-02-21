#ifndef __LINUX_POLLING__H__
#define __LINUX_POLLING__H__

#include "polling/polling.h"
#include <functional>
class LinuxPolling: public Polling {
    public:
        int create_queue() override;
        int add_user(int queue_instance, int user) override;
        void wait_server_events(int queue_instance, 
            std::function<void()> onSuccess,
            std::function<void()> onError) override;
        void wait_worker_events(int queue_instance, 
                int timeout,
                std::function<void(int)> onSuccess,
                std::function<void(int)> onError) override;
        ~LinuxPolling() = default; 
};
#endif  //!__LINUX_POLLING__H__