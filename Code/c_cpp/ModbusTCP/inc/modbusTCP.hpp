#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <errno.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace hcz {
namespace modbusTCP {

struct ModbusTCPFrame {
  uint16_t transactionId;    // 事务标识符 (2 bytes)
  uint16_t protocolId;       // 协议标识符 (2 bytes)
  uint16_t length;           // 长度 (2 bytes)
  uint8_t unitId;            // 单元标识符 (1 byte)
  uint8_t functionCode;      // 功能码 (1 byte)
  std::vector<uint8_t> data; // 数据 (可变长度)
  ModbusTCPFrame();
  ModbusTCPFrame(uint16_t transId, uint8_t unitId, uint8_t funcCode);
};

class CreateFrame {
public:
  // 02
  static ModbusTCPFrame createReadSingleInputStatus(uint16_t transactionId,
                                                    uint16_t address);
  // 02
  static ModbusTCPFrame createReadInputStatus(uint16_t transactionId,
                                              uint16_t startAddress,
                                              uint16_t quantity);
  // 05
  static ModbusTCPFrame createWriteSingleCoil(uint16_t transactionId,
                                              uint16_t address, bool value);
  // 0f
  static ModbusTCPFrame createWriteCoil(uint16_t transactionId,
                                        uint16_t startAddress,
                                        uint16_t quantity,
                                        std::initializer_list<uint8_t> args);
  // 03
  static ModbusTCPFrame createReadHoldingRegisters(uint16_t transactionId,
                                                   uint16_t startAddress,
                                                   uint16_t quantity);
  // 06
  static ModbusTCPFrame createWriteSingleRegister(uint16_t transactionId,
                                                  uint16_t address,
                                                  uint16_t value);
  static std::vector<uint8_t> frameToBytes(const ModbusTCPFrame &frame);
  static ModbusTCPFrame bytesToFrame(const std::vector<uint8_t> &bytes);
  static void printFrame(const ModbusTCPFrame &frame);
};

class ModbusTCPClient {
private:
  int socket_fd;                  // Socket文件描述符
  struct sockaddr_in server_addr; // 服务器地址结构
  bool connected;                 // 连接状态
  std::string server_ip;          // 服务器IP地址
  uint16_t server_port;           // 服务器端口

public:
  ModbusTCPClient();
  ~ModbusTCPClient();

  bool initModbus(const std::string &ip, uint16_t port);
  bool connect();
  bool send(const ModbusTCPFrame &frame);
  bool recv(ModbusTCPFrame &frame);
  void disconnect();
  bool isConnected() const;
};

} // namespace modbusTCP
} // namespace hcz
