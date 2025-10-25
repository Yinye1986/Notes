#pragma once

#include <cstdint>
#include <vector>

namespace modbusrtu {

struct ModbusRtuFrame {
  uint8_t slave_id_;          // 单元标识符 (1 byte)
  uint8_t function_code_;     // 功能码      (1 byte)
  std::vector<uint8_t> data_; // 数据
  uint16_t getCrc16() const;
  std::vector<uint8_t> toBytes() const;
  ModbusRtuFrame(uint8_t slave_id,
                 uint8_t function_code); // Constructor Overloading
};

class BuildFrame {
public:
  // 0x01
  static ModbusRtuFrame BuildReadCoilRequest(uint16_t slave_id,
                                             uint16_t start_address,
                                             uint16_t quantity);
  // 0x02
  static ModbusRtuFrame BuildReadDiscreteInputsRequset(uint16_t slave_id,
                                                       uint16_t start_address,
                                                       uint16_t quantity);
  // 0x03
  static ModbusRtuFrame BuildReadHoldingRegistersRequest(uint16_t slave_id,
                                                         uint16_t start_address,
                                                         uint16_t quantity);
  // 0x04
  static ModbusRtuFrame BuildReadInputRegistersRequest(uint16_t slave_id,
                                                       uint16_t start_address,
                                                       uint16_t quantity);
  // 0x05
  static ModbusRtuFrame
  BuildWriteSingleCoilRequest(uint16_t slave_id, uint16_t address, bool value);
  // 0x06
  static ModbusRtuFrame BuildWriteSingleRegisterRequest(uint16_t slave_id,
                                                        uint16_t address,
                                                        uint16_t value);
  // 0x0f
  static ModbusRtuFrame
  BuildWriteMultipleCoilsRequest(uint16_t slave_id, uint16_t start_address,
                                 uint16_t quantity,
                                 std::initializer_list<uint8_t> args);
  // 0x10
  static ModbusRtuFrame BuildWriteMultipleRegistersRequest();

private:
  BuildFrame() = delete;
  ~BuildFrame() = delete;
};

void printFrame(const ModbusRtuFrame &frame);

} // namespace modbusrtu
