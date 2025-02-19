#ifndef __POLLING_FACTORY__H__
#define __POLLING_FACTORY__H__

#include "polling/polling.h"
#include "polling/mac_polling.h"
#include "polling/linux_polling.h"

#include <memory>
class PollingFactory {
    public:
        static std::unique_ptr<Polling> createPolling() {
            #if defined(__linux__)
                return make_unique<LinuxPolling>();
            #elif (defined(__MACH__) && defined(__APPLE__)) \
                || defined(__FreeBSD__) \
                || defined(__NetBSD__) \
                || defined(__OpenBSD__)
                return std::make_unique<MacPolling>();
              #else
                  #error "Unsupported Platform"
              #endif
        }
};

#endif  //!__POLLING_FACTORY__H__
