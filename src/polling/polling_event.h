#ifndef __POLLING_EVENT_H__
#define __POLLING_EVENT_H__

struct PollingEvent {
    int fd;
    int events; // represent filters set on the event, for example EVFILT_ADD
}