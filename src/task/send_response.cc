#include "task/send_response.h"

#include <unistd.h>
#include <iostream>

void SendResponse::operator() ()
{
    std::string message = serialize_response(response);
    std::cout << message << std::endl;
    write(client_fd, message.c_str(), message.size());
}