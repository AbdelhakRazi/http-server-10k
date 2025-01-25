#include "send_request.h"
#include <unistd.h>
#include <iostream>

void SendResponse::operator() ()
{
    std::string message = "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=UTF-8\r\nContent-Length: 12\r\n\r\nHello world!";
    write(client_fd, message.c_str(), message.size());
}