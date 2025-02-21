#include <iostream>
#include <memory>
#include <signal.h>
#include "logging/trace.h"
#include "server/server.h"
#include "server/tcp_server.h"

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
  set_signal_handler();
  socket_server = std::make_unique<server::TcpServer>(POOL_SIZE);
  socket_server->start();
  return 0;
}
