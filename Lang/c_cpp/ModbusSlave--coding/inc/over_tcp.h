#ifndef OVER_TCP_H
#define OVER_TCP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Modbus TCP 默认端口
#define MODBUS_PORT 502
// 缓冲区大小，Modbus TCP 报文最大约 260 字节
#define MAX_BUFFER_SIZE 512

/**
 * @brief 启动 Modbus TCP Slave 服务器。
 * @param port 监听的端口号。
 * @return true 启动成功并进入主循环。
 * @return false 启动失败。
 * @note 这是一个阻塞函数，调用后将进入无限循环等待连接和数据。
 */
bool transport_tcp_start(int port);

/**
 * @brief 响应 Host (Master)
 * @param client_fd 已连接的客户端的文件描述符
 * @param data 要发送的响应数据
 * @param len 响应数据的长度
 * @return bool 发送成功返回 true
 */
bool transport_tcp_send(int client_fd, const uint8_t *data, size_t len);

#endif // OVER_TCP_H
