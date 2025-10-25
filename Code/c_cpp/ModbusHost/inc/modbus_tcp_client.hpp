#pragma once

#include "modbus_tcp_frame.hpp"

#include <string>

class ModbusTcpClient {
public:
  ModbusTcpClient(const std::string &ip, int port = 502);
  ~ModbusTcpClient();
  bool connectToServer();
  void closeConnection();
  bool sendRequest(const modbustcp::ModbusTcpFrame &frame);
  modbustcp::ModbusTcpFrame readResponse();

  bool isConnected() const;

private:
  std::string server_ip_;
  int server_port_;
  int sock_fd_;
  bool is_connected_;
};
