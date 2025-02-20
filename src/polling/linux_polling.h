#ifndef __LINUX_POLLING__H__
#define __LINUX_POLLING__H__

#include "polling/polling_parameters.h"
#include "polling/polling.h"

class LinuxPolling: public Polling {
    public:
        int create_queue() override;
        int add_user(int queue_instance, int user) override;
        int wait_events(int queue_instance, PollingEvent* events, int timeout) override;
        ~LinuxPolling() = default; 
};
#endif  //!__LINUX_POLLING__H__