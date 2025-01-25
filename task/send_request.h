#ifndef __SEND_REQUEST__H__
#define __SEND_REQUEST__H__
class SendResponse
{
    int client_fd;

public:
    SendResponse(int client_fd) : client_fd{client_fd} {}
    SendResponse(const SendResponse &other) : client_fd{other.client_fd} {}
    SendResponse(SendResponse &&other) : client_fd{other.client_fd} {}
    void operator() ();
    ~SendResponse() = default;
};
#endif //!__SEND_REQUEST__H__
