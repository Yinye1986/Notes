#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8081

int main() {
  // 1. 创建socket
  int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socketFd < 0) {
    printf("create socket error: errno=%d errmsg=%s\n", errno, strerror(errno));
    return 1;
  } else {
    printf("create socket success!\n");
  }

  // 2. 绑定socket
  char ip[] = "127.0.0.1";
  int port = 8081;

  struct sockaddr_in socketAddr;
  memset(&socketAddr, 0, sizeof(socketAddr));
  socketAddr.sin_family = AF_INET;
  socketAddr.sin_addr.s_addr = inet_addr(ip);
  socketAddr.sin_port = htons(port);

  if (bind(socketFd, (struct sockaddr *)&socketAddr, sizeof(socketAddr))) {
    printf("socket bind error: errno=%d, errmsg=%s\n", errno, strerror(errno));
    return 1;
  } else {
    printf("socket bind success!\n");
  }

  // 3. 监听客户端
  if (listen(socketFd, 1024) < 0) {
    printf("socket listen error: errno=%d, errmsg=%s\n", errno,
           strerror(errno));
    return 1;
  } else {
    printf("socket listening ...\n");
  }

  while (1) {
    // 4. 接受客户端
    int connect_fd = accept(socketFd, NULL, NULL);
    if (connect_fd < 0) {
      printf("socket accept error: errno=%d, errmsg=%s\n", errno,
             strerror(errno));
      return 1;
    }
    // else {
    //    printf("socket accept success!\n");
    // }

    // 5. 从客户端接收数据
    char buf[1024] = {0};
    size_t len = recv(connect_fd, buf, sizeof(buf), 0);
    printf("recv: connect_fd=%d msg:\n%s\n\n", connect_fd, buf);

    // 6. 向客户端发送数据
    send(connect_fd, buf, len, 0);
    close(connect_fd);
  }
  close(socketFd);
}
