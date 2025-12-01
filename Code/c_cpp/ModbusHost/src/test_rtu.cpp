#include "modbus_rtu_frame.hpp"

#include <iostream>
#include <vector>

using namespace modbusrtu;

int main() {
  ModbusRtuFrame example01 =
      BuildFrame::BuildReadCoilRequest(0x0001, 0x0001, 0x0008);
  printFrame(example01);
  std::cout << std::endl;
  ModbusRtuFrame example02 =
      BuildFrame::BuildReadDiscreteInputsRequset(2, 2, 10);
  printFrame(example02);
  std::cout << std::endl;
  ModbusRtuFrame example03 =
      BuildFrame::BuildReadHoldingRegistersRequest(3, 3, 1);
  printFrame(example03);
  std::cout << std::endl;
  ModbusRtuFrame example04 =
      BuildFrame::BuildReadInputRegistersRequest(4, 4, 1);
  printFrame(example04);
  std::cout << std::endl;
  ModbusRtuFrame example05 = BuildFrame::BuildWriteSingleCoilRequest(5, 5, 1);
  printFrame(example05);
  std::cout << std::endl;
  ModbusRtuFrame example06 =
      BuildFrame::BuildWriteSingleRegisterRequest(6, 6, 0xFFFF);
  printFrame(example06);
  std::cout << std::endl;
  ModbusRtuFrame example15 =
      BuildFrame::BuildWriteMultipleCoilsRequest(7, 7, 16, {0xFF, 0xFF});
  printFrame(example15);
  std::cout << std::endl;
  ModbusRtuFrame example16 = BuildFrame::BuildWriteMultipleRegistersRequest(
      8, 8, 2, {0xFF, 0xFF, 0xFF, 0xFF});
  printFrame(example16);
  std::cout << std::endl;

  std::vector<uint8_t> b = example16.toBytes();
  ModbusRtuFrame example114 = ModbusRtuFrame::fromBytes(b);
  printFrame(example114);
}
