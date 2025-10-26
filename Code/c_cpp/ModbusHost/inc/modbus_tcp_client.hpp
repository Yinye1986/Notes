#pragma once

#include "modbus_tcp_frame.hpp"

#include <string>

class ModbusTcpClient {
public:
  ModbusTcpClient(const std::string &ip, int port = 502);
  ~ModbusTcpClient();
  bool Connect();
  void Close();
  bool Send(const modbustcp::ModbusTcpFrame &frame);
  modbustcp::ModbusTcpFrame Recv();

  bool isConnected() const;

private:
  std::string server_ip_;
  int server_port_;
  int sock_fd_;
  bool is_connected_;
};
