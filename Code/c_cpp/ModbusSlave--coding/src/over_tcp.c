#include "over_tcp.h"

// POSIX Socket 相关的头文件
#include <stdio.h>       // perror, printf
#include <stdlib.h>      // exit
#include <string.h>      // memset
#include <unistd.h>      // close
#include <sys/socket.h>  // socket, bind, listen, accept, send, recv
#include <netinet/in.h>  // sockaddr_in
#include <arpa/inet.h>   // htons

// 假设我们有一个协议层函数来处理接收到的 Modbus PDU
// 你需要替换为你实际的协议处理函数
extern int modbus_protocol_handle(const uint8_t *request, int req_len, uint8_t *response, int *res_len);


// =========================================================================
// 核心函数：发送数据
// =========================================================================

bool transport_tcp_send(int client_fd, const uint8_t *data, size_t len) {
    ssize_t sent_bytes = send(client_fd, data, len, 0);

    if (sent_bytes == -1) {
        perror("transport_tcp_send: send failed");
        return false;
    }
    if ((size_t)sent_bytes != len) {
        fprintf(stderr, "transport_tcp_send: Sent only %zd of %zu bytes\n", sent_bytes, len);
        // 在实际应用中，这里可能需要重试发送剩余数据
        return false;
    }
    return true;
}


// =========================================================================
// 核心函数：启动服务器
// =========================================================================

bool transport_tcp_start(int port) {
    int listen_fd = -1;
    int client_fd = -1;
    struct sockaddr_in serv_addr;
    uint8_t rx_buffer[MAX_BUFFER_SIZE];

    // 1. 创建 Socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket creation failed");
        return false;
    }

    // 💡 优化：设置 SO_REUSEADDR 允许立即重用端口
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
        close(listen_fd);
        return false;
    }

    // 2. 准备地址结构
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // 监听所有接口 (0.0.0.0)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 端口转换为网络字节序
    serv_addr.sin_port = htons(port);

    // 3. 绑定 (Bind)
    if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        close(listen_fd);
        return false;
    }

    // 4. 监听 (Listen)
    if (listen(listen_fd, 5) < 0) {
        perror("listen failed");
        close(listen_fd);
        return false;
    }

    printf("Modbus TCP Slave listening on port %d...\n", port);

    // 5. 主循环：接受连接并处理数据
    while (true) {
        socklen_t client_len = sizeof(struct sockaddr_in);
        // 阻塞等待新的连接
        client_fd = accept(listen_fd, (struct sockaddr *)&serv_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue; // 继续等待下一个连接
        }

        char *client_ip = inet_ntoa(serv_addr.sin_addr);
        printf("Connection established with %s\n", client_ip);

        // 6. 数据处理循环
        while (true) {
            // Modbus TCP MBAP Header + PDU。我们先读 MBAP 头 (6 字节)
            ssize_t bytes_read = recv(client_fd, rx_buffer, 6, MSG_WAITALL);

            if (bytes_read <= 0) {
                // 连接关闭或错误
                if (bytes_read == 0) {
                    printf("Client %s closed connection.\n", client_ip);
                } else {
                    perror("recv header failed");
                }
                break; // 退出内部循环，关闭 client_fd
            }

            // 7. 解析 PDU 长度（MBAP Header 的第 4 和 5 字节）
            // Modbus 是大端序
            uint16_t pdu_len = (rx_buffer[4] << 8) | rx_buffer[5];
            size_t remaining_len = pdu_len;

            if (pdu_len == 0 || pdu_len > MAX_BUFFER_SIZE - 6) {
                fprintf(stderr, "Invalid PDU length: %hu. Dropping connection.\n", pdu_len);
                break;
            }

            // 8. 读取剩余 PDU 数据
            ssize_t pdu_bytes_read = recv(client_fd, rx_buffer + 6, remaining_len, MSG_WAITALL);

            if (pdu_bytes_read <= 0 || (size_t)pdu_bytes_read != remaining_len) {
                fprintf(stderr, "Incomplete PDU read or connection error.\n");
                break;
            }

            // 9. 调用协议层处理
            int req_len = 6 + pdu_bytes_read; // 总报文长度
            uint8_t tx_buffer[MAX_BUFFER_SIZE];
            int res_len = 0;

            // 假设 modbus_protocol_handle 接收完整报文并填充响应报文
            if (modbus_protocol_handle(rx_buffer, req_len, tx_buffer, &res_len) == 0) {
                // 10. 发送响应
                transport_tcp_send(client_fd, tx_buffer, res_len);
            }
            // 如果协议处理返回错误，我们可能已经发送了异常响应，或者决定不发送任何响应
        }

        // 关闭客户端连接
        close(client_fd);
    }

    // 理论上不会退出循环，但为了完整性，关闭监听套接字
    close(listen_fd);
    return true;
}
