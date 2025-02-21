#ifndef __MAC_POLLING__H__
#define __MAC_POLLING__H__

#include "polling/polling.h"
#include <functional>

class MacPolling: public Polling {
    public:
        int create_queue() override;
        int add_user(int queue_instance, int user) override;
        static constexpr int MAX_SIZE = 1000;
        void wait_server_events(int queue_instance, 
            std::function<void()> onSuccess,
            std::function<void()> onError) override;
        void wait_worker_events(int queue_instance, 
                int timeout,
                std::function<void(int)> onSuccess,
                std::function<void(int)> onError) override;
        ~MacPolling() = default; 
};
#endif  //!__MAC_POLLING__H__