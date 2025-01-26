#include <iostream>
#include <memory>
#include <signal.h>

#include "server/server.h"
#include "server/tcp_server.h"

#define POOL_SIZE 4
namespace
{ // prevent access of server from other files
  std::unique_ptr<Server> server;
}

void sigint_handler(int signal)
{
  if (signal == SIGINT)
  {
    std::cout << "Received sig int, closing sockets" << std::endl;
    server->stop();
  }
}
void set_signal_handler()
{
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.__sigaction_u.__sa_handler = sigint_handler;
  sigaction(SIGINT, &act, NULL);
}
int main()
{
  set_signal_handler();
  server = std::make_unique<TcpServer>(POOL_SIZE);
  server->start();
  return 0;
}
