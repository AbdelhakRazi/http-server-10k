#ifndef __POLLING__H__
#define __POLLING__H__

#include "polling/event_type.h"
#include <functional>
class Polling {
    public:
        virtual int create_queue() = 0;
        virtual int add_user(int queue_instance, int user) = 0;
        virtual void wait_events(int queue_instance, 
            int timeout, 
            EventType event_type,
            std::function<void()> onSuccess,
            std::function<void()> onError) = 0;
        virtual ~Polling() = default; 
};
#endif  //!__POLLING__H__