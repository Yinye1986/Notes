#include "socket.h"
using namespace lynx::socket;

int main() {
  logger::instance()->open("./server.log");
  c_socket server;
  server.bind("127.0.0.1", 8081);
  server.listen(1024);
  while (1) {
    server.accept();
  }
}
