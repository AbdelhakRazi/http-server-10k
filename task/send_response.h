#ifndef __SEND_RESPONSE__H__
#define __SEND_RESPONSE__H__

struct SendResponse
{
    int client_fd;

public:
    SendResponse(int client_fd) : client_fd{client_fd} {}
    SendResponse(const SendResponse &other) : client_fd{other.client_fd} {}
    SendResponse(const SendResponse &&other) : client_fd{other.client_fd} {}
    void operator() ();
    ~SendResponse() = default;
};
#endif //!__SEND_REQUEST__H__
