#pragma once

class Server {
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual ~Server() = default;
};