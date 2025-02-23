#include "task/read_request.h"

#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

#include "http/http_request.h"
#include "parser/http_parser.h"
#include "logging/trace.h"
#include "task/send_response.h"
#include "exceptions/http_exception.h"


// when reading we have following scenarios:
// receive all data: OK, no need to read
// receive one segment of data, need to wait for other segments
// segments might take time to receive, so recv might return 0 bytes, so we'll have to wait for more.
// when waiting for data, due to congestion or network issues, it'll return -1 with EAGAIN or EWOULDBLOCK
void ReadRequest::operator() ()
{
    char buffer[4196] = {0};
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
    if(bytes_read == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
            TRACE_DEBUG("Error brooooo");
            close(client_fd);
            current_fds.erase(client_fd);
            return;
    }
    if (bytes_read == 0)
    {
        std::cout << "Client shutdown: " << client_fd << std::endl;
        current_fds.erase(client_fd);
        close(client_fd); // stops automatically kevent kernel from monitoring the file descriptor
        return;
    }
    HttpRequest request;
    try {
        request = parse_http_request(buffer, bytes_read);
    }
    catch(HttpException exception) {
        TRACE_DEBUG("Exception thrown: %s", exception.what());
        return;
    }
    std::string optional_headers;
    if(request.optional_headers.has_value()) {
        optional_headers = request.optional_headers.value();
    }
    std::string body;
    if(request.body.has_value()) {
        body = request.body.value();
    }
    TRACE_DEBUG("Method: %s", request.method.c_str());
    TRACE_DEBUG("URI: %s", request.uri.c_str());
    TRACE_DEBUG("Version: %s", request.version.c_str());
    TRACE_DEBUG("Optional headers: %s", optional_headers.c_str());
    TRACE_DEBUG("Body: %s", body.c_str());


    // read succesfully, write
    SendResponse{static_cast<int>(client_fd)}();
}
