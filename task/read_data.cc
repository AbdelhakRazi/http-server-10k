#include "read_data.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sys/event.h>
#include <arpa/inet.h>
#include "../request/http_request.h"
#include "../parser/http_parser.h"

void ReadData::operator() ()
{
    char buffer[1024] = {0};
    int read_status = read(client_fd, buffer, sizeof(buffer));
    if (read_status < 0)
    {
        // since socket async, data might not be received yet, add to monitor again
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            struct kevent client_monitor;
            EV_SET(&client_monitor, client_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
            kevent(kqueue_instance, &client_monitor, 1, nullptr, 0, nullptr); // direct add */
        }
        else {
            perror("failed to read");
        }
        return;
    }
    else if (read_status == 0)
    {
        std::cout << "Stop monitoring client: " << client_fd << std::endl;
        current_fds.erase(client_fd);
        close(client_fd); // stops automatically kevent kernel from monitoring the file descriptor
        return;
    } // it means select updated the client, but returned 0, in this case the client has disconnected
    std::cout << "----------------------------------------------" << std::endl;
    HttpRequest httpRequest{parse_http_request(buffer, read_status)};
    /* std::cout << "----------------------- Header ----------------------" << std::endl;
    std::cout << "Method: " << httpRequest.header.method << std::endl;
    std::cout << "Endpoint: " << httpRequest.header.endpoint << std::endl;
    std::cout << "Version: " << httpRequest.header.version << std::endl;
    std::cout << "----------------------- Body ----------------------" << std::endl;
    std::cout << httpRequest.body.payload << std::endl;
    std::cout << "----------------------------------------------" << std::endl; */
}
