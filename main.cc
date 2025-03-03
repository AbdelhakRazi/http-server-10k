#include <iostream>
#include <memory>
#include <signal.h>
#include "trace/trace.h"
#include "server/server.h"
#include "server/tcp_server.h"
#include "app/app.h"
#include "http/http_request.h"
#include "http/http_response.h"

#define POOL_SIZE 5
namespace
{ // prevent access of server from other files
  std::unique_ptr<server::Server> socket_server;
}

void sigint_handler(int signal)
{
  if (signal == SIGINT)
  {
    TRACE_DEBUG("Received SIGINT");
    socket_server->stop();
  }
}
void set_signal_handler()
{
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = sigint_handler;
  sigaction(SIGINT, &act, NULL);
}
int main()
{
  App app;
  app.get("/items", [](const HttpRequest& request) {
    HttpResponse response;
    response.status_code = 200;
    response.status_message = "OK";
    response.version = "HTTP/1.1";
    response.body = "{message: I exist brother}";
    response.optional_headers = "Content-Type: text/plain; charset=UTF-8\r\nContent-Length: " + std::to_string(response.body.value().length());
    return response;
  });
  app.put("/items", [](auto request) {
    HttpResponse response;
    response.status_code = 200;
    response.status_message = "OK";
    response.version = "HTTP/1.1";
    response.body = "{message: Thank you for sending}";
    response.optional_headers = "Content-Type: text/plain; charset=UTF-8\r\nContent-Length: " + std::to_string(response.body.value().length());
    return response;
  });
  set_signal_handler();
  socket_server = std::make_unique<server::TcpServer>(POOL_SIZE);
  socket_server->start();
  return 0;
}
