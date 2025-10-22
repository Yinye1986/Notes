#include "modbusTCP.hpp"
using namespace hcz::modbusTCP;

int main() {
  // 02
  ModbusTCPFrame a =
      CreateFrame::createReadInputStatus(1, 1, 8); // 事务標識符，起始地址，個數
  ModbusTCPFrame b =
      CreateFrame::createWriteSingleCoil(2, 1, 1); // 事務標識符，地址，值(bool)
  ModbusTCPFrame c = CreateFrame::createWriteCoil(3, 1, 9, {0x20, 0x01});

  CreateFrame::printFrame(a);
  CreateFrame::printFrame(b);
  CreateFrame::printFrame(c);
  return 0;
}
