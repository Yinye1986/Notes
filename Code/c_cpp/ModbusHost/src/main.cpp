#include "modbus_tcp_client.hpp"
#include "modbus_tcp_frame.hpp"
#include <iostream>
#include <vector>

using namespace modbustcp;

int main() {
  ModbusTcpClient client("192.168.123.52", 502);

  if (!client.Connect()) {
    std::cerr << "Failed to connect slave." << std::endl;
    return 1;
  }
  uint16_t transaction_id = 1;
  ModbusTcpFrame requests = BuildFrame::BuildReadHoldingRegistersRequest(
      transaction_id, 1, 1);

  if (!client.Send(requests)) {
    std::cerr << "Failed to send requests." << std::endl;
    client.Close();
    return 1;
  }

  ModbusTcpFrame response = client.Recv();

  if (response.transaction_id_ == transaction_id) {
    std::cout << "requests: ";
    printFrame(requests);
    std::cout << "response: " << std::endl;
    printFrame(response);
  } else if (response.transaction_id_ == 0 && response.data_.empty()) {
    std::cerr << "Failed to read a valid response." << std::endl;
  } else {
    std::cerr << "Transaction ID mismatch!" << std::endl;
    std::cout << "received: " << std::endl;
    printFrame(response);
  }

  // 7. 关闭连接 (析构函数也会自动调用)
  client.Close();

  return 0;
}
