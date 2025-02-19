#ifndef __POLLING_PARAMETERS__H__
#define __POLLING_PARAMETERS__H__

#include "polling/mac_paremeters.h"
#include "polling/linux_parameters.h"


#if defined(__linux__)
    using PollingParameters = LinuxParameters;
#elif (defined(__MACH__) && defined(__APPLE__)) \
  || defined(__FreeBSD__) \
  || defined(__NetBSD__) \
  || defined(__OpenBSD__)
    using PollingParameters = MacParameters;
#else
    #error "Unsupported Platform"
#endif
#endif  //!__POLLING_PARAMETERS__H__