#pragma once

#include <string>
using std::string;
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../utility/logger.h"
using namespace lynx::utility;

namespace lynx {
namespace socket {
class c_socket {
public:
  c_socket();
  ~c_socket();
  bool bind(const string &ip, int port);
  bool listen(int backlog);
  bool connect(const string &ip, int port);
  int accept();
  int send(const char *buf, int len);
  int recv(char *buf, int len);
  void close();

protected:
  string m_ip;
  int m_port;
  int m_socketFd;
  int m_connectFd;
};
} // namespace socket
} // namespace lynx
