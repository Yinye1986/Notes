#include "modbus_tcp_client.hpp"
#include <arpa/inet.h> // inet_pton
#include <cstring>     // memset
#include <iostream>
#include <sys/socket.h> // socket, connect, send, recv
#include <unistd.h>     // close
#include <vector>

ModbusTcpClient::ModbusTcpClient(const std::string &ip, int port)
    : server_ip_(ip), server_port_(port), sock_fd_(-1), is_connected_(false) {}

ModbusTcpClient::~ModbusTcpClient() { this->Close(); }

void ModbusTcpClient::Close() {
  if (is_connected_) {
    close(sock_fd_);
    is_connected_ = false;
    sock_fd_ = -1;
    std::cout << "Connection closed." << std::endl;
  }
}

bool ModbusTcpClient::Connect() {
  if (is_connected_) {
    std::cerr << "Already connected." << std::endl;
    return true;
  }

  sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd_ < 0) {
    std::cerr << "Failed to create socket." << std::endl;
    return false;
  }

  // 2. 准备服务器地址结构
  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr)); // 初始化为 0
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port_); // 转换为网络字节序 (Big-Endian)

  // 3. 将 IP 地址字符串转换为网络格式
  // inet_pton: "presentation to network"
  if (inet_pton(AF_INET, server_ip_.c_str(), &serv_addr.sin_addr) <= 0) {
    std::cerr << "inet_pton() failed" << std::endl;
    close(sock_fd_);
    sock_fd_ = -1;
    return false;
  }

  if (connect(sock_fd_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "connect() failed" << std::endl;
    close(sock_fd_);
    sock_fd_ = -1;
    return false;
  }

  std::cout << "Connected to " << server_ip_ << ":" << server_port_
            << std::endl;
  is_connected_ = true;
  return true;
}

bool ModbusTcpClient::Send(const modbustcp::ModbusTcpFrame &frame) {
  if (!is_connected_) {
    std::cerr << "Not connected." << std::endl;
    return false;
  }

  std::vector<uint8_t> bytes_to_send = frame.toBytes();
  ssize_t bytes_sent =
      send(sock_fd_, bytes_to_send.data(), bytes_to_send.size(), 0);

  if (bytes_sent < 0) {
    std::cerr << "send() failed" << std::endl;
    Close(); // 发送失败，可能连接已断开
    return false;
  }

  if (static_cast<size_t>(bytes_sent) != bytes_to_send.size()) {
    std::cerr << "Incomplete send." << std::endl;
    return false;
  }

  return true;
}

modbustcp::ModbusTcpFrame ModbusTcpClient::Recv() {
  if (!is_connected_) {
    std::cerr << "Not connected." << std::endl;
    return modbustcp::ModbusTcpFrame(); // 返回空
  }

  // Modbus TCP 响应的 MBAP 头至少有 6 个字节
  unsigned char mbap_header[6];
  ssize_t bytes_read = 0;

  // 读取固定的 6 字节 MBAP 头
  // (Transaction ID, Protocol ID, Length)
  // 使用 MSG_WAITALL 来确保读满 6 字节再返回
  bytes_read = recv(sock_fd_, mbap_header, 6, MSG_WAITALL);

  if (bytes_read <= 0) {
    if (bytes_read == 0) {
      std::cerr << "Connection closed by server." << std::endl;
    } else {
      perror("recv() header failed");
    }
    Close();
    return modbustcp::ModbusTcpFrame();
  }

  // 2. 从 MBAP 头中解析 "Length" 字段
  //    它在第 4 和 第 5 字节
  //    它指示 *后面* 还有多少字节 (Unit ID + PDU)
  uint16_t pdu_length = (mbap_header[4] << 8) | mbap_header[5];
  if (pdu_length == 0) {
    std::cerr << "Invalid PDU length (0)." << std::endl;
    return modbustcp::ModbusTcpFrame();
  }
  if (pdu_length > 256) { // PDU (UnitID + Func + Data) 最大 1 + 1 + 253 = 255
    std::cerr << "PDU length too large: " << pdu_length << std::endl;
    // 仍然尝试读取，以清空缓冲区，但可能出错
  }

  // 3. 准备接收剩余的 PDU
  std::vector<uint8_t> all_bytes;
  all_bytes.insert(all_bytes.end(), mbap_header,
                   mbap_header + 6); // 先把头放进去

  std::vector<uint8_t> pdu_data(pdu_length);
  bytes_read = recv(sock_fd_, pdu_data.data(), pdu_length, MSG_WAITALL);

  if (bytes_read <= 0) {
    if (bytes_read == 0) {
      std::cerr << "Connection closed by server while reading PDU."
                << std::endl;
    } else {
      perror("recv() PDU failed");
    }
    Close();
    return modbustcp::ModbusTcpFrame();
  }

  if (static_cast<size_t>(bytes_read) != pdu_length) {
    std::cerr << "Incomplete PDU read." << std::endl;
    return modbustcp::ModbusTcpFrame();
  }

  all_bytes.insert(all_bytes.end(), pdu_data.begin(), pdu_data.end());
  return modbustcp::ModbusTcpFrame::fromBytes(all_bytes);
}

bool ModbusTcpClient::isConnected() const { return is_connected_; }
