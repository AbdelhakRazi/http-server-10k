#ifndef __SERVER__H__
#define __SERVER__H__

namespace server {
class Server {
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual ~Server() = default;
};
} // namespace server
#endif  //!__SERVER__H__
