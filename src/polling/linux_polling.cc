#include "linux_polling.h"

int LinuxPolling::create_queue()
{
    return 0;
}

int LinuxPolling::add_user(int queue_instance, int user)
{
    return 0;
}

void LinuxPolling::wait_events(int queue_instance, int timeout, EventType event_type, std::function<void()> onSuccess, std::function<void()> onError)
{
}
