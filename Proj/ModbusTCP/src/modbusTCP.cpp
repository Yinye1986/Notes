#include "modbusTCP.h"
#include <initializer_list>
#include <iomanip>
#include <iostream>

using namespace hcz::modbusTCP;

// 結構體的構造函數
ModbusTCPFrame::ModbusTCPFrame()
    : transactionId(0), protocolId(0), length(0), unitId(0), functionCode(0) {}
ModbusTCPFrame::ModbusTCPFrame(uint16_t transId, uint8_t unitId,
                               uint8_t funcCode)
    : transactionId(transId), protocolId(0), unitId(unitId),
      functionCode(funcCode) {}

// CreateFrame類實現
ModbusTCPFrame CreateFrame::createReadSingleInputStatus(uint16_t transactionId,
                                                        uint16_t address) {
  ModbusTCPFrame frame(transactionId, 0x01, 0x02);
  frame.data.push_back((address >> 8) & 0xFF);
  frame.data.push_back(address & 0xFF);
  frame.data.push_back((0x0001 >> 8) & 0xFF);
  frame.data.push_back(0x0001 & 0xFF);
  frame.length = 1 + 1 + frame.data.size();
  return frame;
}

ModbusTCPFrame CreateFrame::createReadInputStatus(uint16_t transactionId,
                                                  uint16_t startAddress,
                                                  uint16_t quantity) {
  ModbusTCPFrame frame(transactionId, 0x01, 0x02);
  frame.data.push_back((startAddress >> 8) & 0xFF);
  frame.data.push_back(startAddress & 0xFF);
  frame.data.push_back((quantity >> 8) & 0xFF);
  frame.data.push_back(quantity & 0xFF);
  frame.length = 1 + 1 + frame.data.size();
  return frame;
}

ModbusTCPFrame CreateFrame::createWriteSingleCoil(uint16_t transactionId,
                                                  uint16_t address,
                                                  bool value) {
  ModbusTCPFrame frame(transactionId, 0x01, 0x05);
  frame.data.push_back((address >> 8) & 0xFF);
  frame.data.push_back(address & 0xFF);
  if (value == 1) {
    frame.data.push_back(0xFF);
    frame.data.push_back(0x00);
  } else {
    frame.data.push_back(0x00);
    frame.data.push_back(0x00);
  }
  frame.length = 1 + 1 + frame.data.size();
  return frame;
}

ModbusTCPFrame
CreateFrame::createWriteCoil(uint16_t transactionId, uint16_t startAddress,
                             uint16_t quantity,
                             std::initializer_list<uint8_t> args) {
  ModbusTCPFrame frame(transactionId, 0x01, 0x0F);
  frame.data.push_back((startAddress >> 8) & 0xFF);
  frame.data.push_back(startAddress & 0xFF);
  frame.data.push_back((quantity >> 8) & 0xFF);
  frame.data.push_back(quantity & 0xFF);
  frame.data.insert(frame.data.end(), args.begin(), args.end());
  frame.length = 1 + 1 + frame.data.size();
  return frame;
}

ModbusTCPFrame CreateFrame::createReadHoldingRegisters(uint16_t transactionId,
                                                       uint16_t startAddress,
                                                       uint16_t quantity) {
  ModbusTCPFrame frame(transactionId, 0x01, 0x03);
  frame.data.push_back(
      (startAddress >> 8) &
      0xFF); // 先右移丟棄低位，再按位與，確保高八位被丟棄(爲防止算數右移)
  frame.data.push_back(startAddress & 0xFF);    // 低字节
  frame.data.push_back((quantity >> 8) & 0xFF); // 高字节
  frame.data.push_back(quantity & 0xFF);        // 低字节
  frame.length = 1 + 1 + frame.data.size();
  return frame;
}

ModbusTCPFrame CreateFrame::createWriteSingleRegister(uint16_t transactionId,
                                                      uint16_t address,
                                                      uint16_t value) {
  ModbusTCPFrame frame(transactionId, 0x01, 0x06);
  frame.data.push_back((address >> 8) & 0xFF);
  frame.data.push_back(address & 0xFF);
  frame.data.push_back((value >> 8) & 0xFF);
  frame.data.push_back(value & 0xFF);
  frame.length = 1 + 1 + frame.data.size();
  return frame;
}

std::vector<uint8_t> CreateFrame::frameToBytes(const ModbusTCPFrame &frame) {
  std::vector<uint8_t> bytes;
  bytes.reserve(7 + frame.data.size()); // MBAP头(7字节) + 数据
  bytes.push_back((frame.transactionId >> 8) & 0xFF);
  bytes.push_back(frame.transactionId & 0xFF);
  bytes.push_back((frame.protocolId >> 8) & 0xFF);
  bytes.push_back(frame.protocolId & 0xFF);
  bytes.push_back((frame.length >> 8) & 0xFF);
  bytes.push_back(frame.length & 0xFF);
  bytes.push_back(frame.unitId);
  bytes.push_back(frame.functionCode);
  bytes.insert(bytes.end(), frame.data.begin(), frame.data.end());
  return bytes;
}

ModbusTCPFrame CreateFrame::bytesToFrame(const std::vector<uint8_t> &bytes) {
  ModbusTCPFrame frame;
  if (bytes.size() < 8) {
    return frame;
  }
  frame.transactionId = (bytes[0] << 8) | bytes[1];
  frame.protocolId = (bytes[2] << 8) | bytes[3];
  frame.length = (bytes[4] << 8) | bytes[5];
  frame.unitId = bytes[6];
  frame.functionCode = bytes[7];
  if (bytes.size() > 8) {
    frame.data.assign(bytes.begin() + 8, bytes.end());
  }
  return frame;
}

// 显示帧内容 (用于调试)
void CreateFrame::printFrame(const ModbusTCPFrame &frame) {
  std::cout << "Modbus TCP Frame:" << std::endl;
  std::cout << "  Transaction ID: 0x" << std::hex << std::setfill('0')
            << std::setw(4) << frame.transactionId << std::dec << std::endl;
  std::cout << "  Protocol ID:    0x" << std::hex << std::setfill('0')
            << std::setw(4) << frame.protocolId << std::dec << std::endl;
  std::cout << "  Length:         0x" << std::hex << std::setfill('0')
            << std::setw(4) << frame.length << std::dec << std::endl;
  std::cout << "  Unit ID:        0x" << std::hex << std::setfill('0')
            << std::setw(2) << (int)frame.unitId << std::dec << std::endl;
  std::cout << "  Function Code:  0x" << std::hex << std::setfill('0')
            << std::setw(2) << (int)frame.functionCode << std::dec << std::endl;

  if (!frame.data.empty()) {
    std::cout << "  Data:           ";
    for (size_t i = 0; i < frame.data.size(); ++i) {
      if (i > 0)
        std::cout << " ";
      std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (int)frame.data[i] << std::dec;
    }
    std::cout << std::endl;
  }
}

// Modbus TCP客户端类实现
ModbusTCPClient::ModbusTCPClient()
    : socket_fd(-1), connected(false), server_port(0) {
  memset(&server_addr, 0, sizeof(server_addr));
}

ModbusTCPClient::~ModbusTCPClient() { disconnect(); }

bool ModbusTCPClient::initModbus(const std::string &ip, uint16_t port) {
  server_ip = ip;
  server_port = port;

  // 创建socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    std::cerr << "创建socket失败: " << strerror(errno) << std::endl;
    return false;
  }

  // 设置服务器地址
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
    std::cerr << "无效的IP地址: " << ip << std::endl;
    close(socket_fd);
    socket_fd = -1;
    return false;
  }

  return true;
}

bool ModbusTCPClient::connect() {
  if (socket_fd == -1) {
    std::cerr << "Socket未初始化" << std::endl;
    return false;
  }

  if (::connect(socket_fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
    std::cerr << "连接失败: " << strerror(errno) << std::endl;
    return false;
  }

  connected = true;
  std::cout << "成功连接到服务器 " << server_ip << ":" << server_port
            << std::endl;
  return true;
}

bool ModbusTCPClient::send(const ModbusTCPFrame &frame) {
  if (!connected || socket_fd == -1) {
    std::cerr << "未连接到服务器" << std::endl;
    return false;
  }
  std::vector<uint8_t> bytes = hcz::modbusTCP::CreateFrame::frameToBytes(frame);
  ssize_t sent_bytes = ::send(socket_fd, bytes.data(), bytes.size(), 0);
  if (sent_bytes == -1) {
    std::cerr << "发送数据失败: " << strerror(errno) << std::endl;
    return false;
  }

  if (sent_bytes != static_cast<ssize_t>(bytes.size())) {
    std::cerr << "发送的数据量不完整，期望发送 " << bytes.size()
              << " 字节，实际发送 " << sent_bytes << " 字节" << std::endl;
    return false;
  }
  return true;
}

bool ModbusTCPClient::recv(ModbusTCPFrame &frame) {
  if (!connected || socket_fd == -1) {
    std::cerr << "未连接到服务器" << std::endl;
    return false;
  }

  uint8_t header[8];
  ssize_t received = ::recv(socket_fd, header, sizeof(header), MSG_WAITALL);
  if (received == -1) {
    std::cerr << "接收数据失败: " << strerror(errno) << std::endl;
    return false;
  }

  if (received != sizeof(header)) {
    std::cerr << "接收的头部数据不完整" << std::endl;
    return false;
  }

  // 解析头部信息
  uint16_t transactionId = (header[0] << 8) | header[1];
  uint16_t protocolId = (header[2] << 8) | header[3];
  uint16_t length = (header[4] << 8) | header[5];
  uint8_t unitId = header[6];
  uint8_t functionCode = header[7];

  // 根据长度接收剩余数据
  uint16_t dataLen = length - 2; // 减去unitId和functionCode的长度
  std::vector<uint8_t> data(dataLen);
  if (dataLen > 0) {
    received = ::recv(socket_fd, data.data(), dataLen, MSG_WAITALL);
    if (received == -1) {
      std::cerr << "接收数据失败: " << strerror(errno) << std::endl;
      return false;
    }

    if (received != static_cast<ssize_t>(dataLen)) {
      std::cerr << "接收的数据量不完整，期望接收 " << dataLen
                << " 字节，实际接收 " << received << " 字节" << std::endl;
      return false;
    }
  }

  // 构建响应帧
  frame.transactionId = transactionId;
  frame.protocolId = protocolId;
  frame.length = length;
  frame.unitId = unitId;
  frame.functionCode = functionCode;
  frame.data = data;

  return true;
}

void ModbusTCPClient::disconnect() {
  if (socket_fd != -1) {
    close(socket_fd);
    socket_fd = -1;
  }
  connected = false;
  std::cout << "已断开与服务器的连接" << std::endl;
}

bool ModbusTCPClient::isConnected() const { return connected; }
