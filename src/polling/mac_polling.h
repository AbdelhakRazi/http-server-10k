#ifndef __MAC_POLLING__H__
#define __MAC_POLLING__H__

#include "polling/polling_parameters.h"
#include "polling/polling.h"

class MacPolling: public Polling {
    public:
        int create_queue() override;
        int add_user(int queue_instance, int user) override;
        int wait_events(int queue_instance, PollingEvent* events, int timeout) override;
        ~MacPolling() = default; 
};
#endif  //!__MAC_POLLING__H__