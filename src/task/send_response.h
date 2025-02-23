#ifndef __SEND_RESPONSE__H__
#define __SEND_RESPONSE__H__

#include "http/http_response.h"
struct SendResponse
{
    int client_fd;
    HttpResponse response;

public:
    SendResponse(int client_fd, const HttpResponse& response) : client_fd{client_fd}, response{response} {}
    SendResponse(const SendResponse &other) : client_fd{other.client_fd} {}
    SendResponse(const SendResponse &&other) : client_fd{other.client_fd} {}
    void operator() ();
    ~SendResponse() = default;
};
#endif //!__SEND_REQUEST__H__
