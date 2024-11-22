#include "socket.h"

using namespace lynx::socket;

c_socket::c_socket() : m_ip(""), m_port(0), m_socketFd(0), m_connectFd(0) {
  m_socketFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_socketFd < 0) {
    error("create socket error: errno=%d errmsg=%s", errno, strerror(errno));
  } else {
    debug("create socket success!");
  }
}
c_socket::~c_socket() { close(); }
bool c_socket::bind(const string &ip, int port) {
  // 2. 绑定socket
  struct sockaddr_in socketAddr;
  memset(&socketAddr, 0, sizeof(socketAddr));
  socketAddr.sin_family = AF_INET;
  if (ip.empty()) {
    socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  socketAddr.sin_addr.s_addr = inet_addr(ip.c_str());
  socketAddr.sin_port = htons(port);
  m_ip = ip;
  m_port = port;
  if (::bind(m_socketFd, (struct sockaddr *)&socketAddr, sizeof(socketAddr))) {
    error("socket bind error: errno=%d, errmsg=%s", errno, strerror(errno));
    return false;
  } else {
    debug("socket bind success: ip=%s, port=%d", ip.c_str(), port);
    return true;
  }
}
bool c_socket::listen(int backlog) {
  if (::listen(m_socketFd, 1024) < 0) {
    error("socket listen error: errno=%d, errmsg=%s", errno, strerror(errno));
    return false;
  } else {
    debug("socket listening ...");
    return false;
  }
}
bool c_socket::connect(const string &ip, int port) {
  struct sockaddr_in sockAddr;
  memset(&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr.s_addr = ::inet_addr(ip.c_str());
  sockAddr.sin_port = htons(port);
  m_ip = ip;
  m_port = port;
  if (::connect(m_socketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) <
      0) {
    error("socket connect error: errno=%d, errmsg=%s", errno, strerror(errno));
    return false;
  } else {
    debug("socket connect success!");
    return true;
  }
}
int c_socket::accept() {
  int connectFd = ::accept(m_socketFd, NULL, NULL);
  m_connectFd = connectFd;
  if (connectFd < 0) {
    error("socket accept error: errno=%d, errmsg=%s", errno, strerror(errno));
    return false;
  } else {
    debug("socket accept success!");
    return connectFd;
  }
}
int c_socket::send(const char *buf, int len) {
  return ::send(m_socketFd, buf, len, 0);
}
int c_socket::recv(char *buf, int len) {
  return ::recv(m_socketFd, buf, len, 0);
}
void c_socket::close() {
  if (m_socketFd > 0) {
    ::close(m_socketFd);
    m_socketFd = 0;
  }
  if (m_connectFd > 0) {
    ::close(m_connectFd);
    m_connectFd = 0;
  }
}
