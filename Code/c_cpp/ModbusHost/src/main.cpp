#include "modbus_tcp_client.hpp"
#include "modbus_tcp_frame.hpp" // 你的头文件
#include <iostream>
#include <vector>

// 使用你的命名空间
using namespace modbustcp;

int main() {
  // 1. 初始化客户端，指向你的服务器 IP 和端口
  //    这里使用 "127.0.0.1" (localhost) 作为示例
  ModbusTcpClient client("127.0.0.1", 502);

  // 2. 连接服务器
  if (!client.connectToServer()) {
    std::cerr << "Failed to connect to Modbus server." << std::endl;
    return 1;
  }

  // 3. 使用你的 BuildFrame 构造一个请求
  //    例如: 读保持寄存器 (0x03)
  //    事务 ID: 1 (每次请求应递增)
  //    起始地址: 0
  //    寄存器数量: 10
  uint16_t transaction_id = 1;
  ModbusTcpFrame request = BuildFrame::BuildReadHoldingRegistersRequest(
      transaction_id, 0, 10, 0x01);

  std::cout << "Sending Request: " << std::endl;
  printFrame(request); // 使用你的打印函数

  // 4. 发送请求
  if (!client.sendRequest(request)) {
    std::cerr << "Failed to send request." << std::endl;
    client.closeConnection();
    return 1;
  }

  // 5. 阻塞读取响应
  ModbusTcpFrame response = client.readResponse();

  // 6. 检查响应是否有效 (例如，检查事务 ID 是否匹配)
  if (response.transaction_id_ == transaction_id) {
    std::cout << "Received Response: " << std::endl;
    printFrame(response);

    // 你可以在这里进一步解析 response.data_
    // ...

  } else if (response.transaction_id_ == 0 && response.data_.empty()) {
    // 可能是 readResponse() 内部出错返回的空 frame
    std::cerr << "Failed to read a valid response." << std::endl;
  } else {
    std::cerr << "Transaction ID mismatch!" << std::endl;
    std::cout << "Received: " << std::endl;
    printFrame(response);
  }

  // 7. 关闭连接 (析构函数也会自动调用)
  client.closeConnection();

  return 0;
}
