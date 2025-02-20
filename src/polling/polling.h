#ifndef __POLLING__H__
#define __POLLING__H__

#include "polling/polling_parameters.h"

class Polling {
    public:
        virtual int create_queue() = 0;
        virtual int add_user(int queue_instance, int user) = 0;
        virtual int wait_events(int queue_instance, PollingEvent* events, int timeout) = 0;
        virtual ~Polling() = default; 
};
#endif  //!__POLLING__H__