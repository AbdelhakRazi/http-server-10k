#ifndef __SERVER__H__
#define __SERVER__H__

class Server {
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual ~Server() = default;
};

#endif  //!__SERVER__H__
