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
        printf("create socket error: errno=%d errmsg=%s\n", errno,
               strerror(errno));
        return 1;
    } else {
        printf("create socket success!\n");
    }

    // 2. 连接服务端
    char ip[] = "127.0.0.1";
    int port = 8081;

    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    if (connect(socketFd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        printf("socket connect error: errno=%d, errmsg=%s\n", errno,
               strerror(errno));
        return 1;
    } else {
        printf("socket connect success!\n");
    }

    // 3. 向服务端发送数据
    char data[] = "hello server";
    send(socketFd, data, strlen(data), 0);

    // 4. 从服务端接收数据
    char buf[1024] = {0};
    recv(socketFd, buf, sizeof(buf), 0);
    printf("recv: %s\n", buf);

    // 5. 关闭socket
    close(socketFd);
    return 0;
}
