#include "modbus_rtu_frame.hpp"

#include <iostream>

using namespace modbusrtu;

int main() {
    // 02
    ModbusRtuFrame example02 = BuildFrame::BuildReadDiscreteInputsRequset(0x01, 0x0001, 0x0008);
    printFrame(example02);
    std::cout << std::endl;
    // 03
    ModbusRtuFrame example03 = BuildFrame::BuildReadHoldingRegistersRequest(1, 1, 1);
    printFrame(example03);
    std::cout << std::endl;
    // 04
}
