#pragma once

#include "modbus_tcp_client.hpp"
#include "modbus_tcp_frame.hpp"
#include <string>

namespace modbustcp {

class ModbusTcpClient {
public:
  /**
   * @brief 构造函数
   * @param ip 服务器 IP 地址
   * @param port 服务器端口 (Modbus TCP 默认为 502)
   */
  ModbusTcpClient(const std::string &ip, int port = 502);
  ~ModbusTcpClient();

  bool Connect(const std::string &ip, int port);
  void Disconnect();

  bool SendFrame();
  bool RecvFrame(ModbusTcpFrame &frame);

  bool IsConnected() const;

private:
  int socket_fd_;
  bool connected_;
  std::string ip_;
  int port_;

  bool InitializeSocket();
};

} // namespace modbustcp
