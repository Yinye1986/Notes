#include "modbus_tcp_frame.hpp"
#include <iostream>

using namespace modbustcp;

int main() {

    // 02
    ModbusTcpFrame example02 = BuildFrame::BuildReadDiscreteInputsRequset(0x0000, 0x0001, 0x0010);
    printFrame(example02);
    std::cout << std::endl;
    // 03
    ModbusTcpFrame example03 = BuildFrame::BuildReadHoldingRegistersRequest(1, 1, 1);
    printFrame(example03);


}
