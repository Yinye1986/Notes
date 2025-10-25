#pragma once

#include <cstdint>
#include <vector>
#include <initializer_list>

namespace modbustcp {

struct ModbusTcpFrame {
  uint16_t transaction_id_;             // 事务标识符 (2 bytes)
  uint16_t protocol_id_ = 0x0000;       // 协议标识符 (2 bytes)
  uint16_t length_;                     // 后面除了CRC的所有字节总数 (2 byte)
  uint8_t unit_id_;                     // 单元标识符 (1 byte)
  uint8_t function_code_;               // 功能码      (1 byte)
  std::vector<uint8_t> data_;           // 数据
  std::vector<uint8_t> toBytes() const; // const表示不会修改当前实例
  static ModbusTcpFrame fromBytes(
      const std::vector<uint8_t> &bytes); // static表示不依赖实例可直接调用,
                                          // 传入一个const引用(不可变引用)
  ModbusTcpFrame() = default;             // 使用默认方法
  ModbusTcpFrame(uint16_t transaction_id, uint8_t unit_id,
                 uint8_t function_code); // Constructor Overloading
};

class BuildFrame {
public:
  // 0x01 读: Q 区 quantity: 位数
  static ModbusTcpFrame BuildReadCoilRequest(uint16_t transaction_id,
                                             uint16_t start_address,
                                             uint16_t quantity,
                                             uint8_t unit_id = 0x01);
  // 0x02 读: I 区 quantity: 位数
  static ModbusTcpFrame BuildReadDiscreteInputsRequset(uint16_t transaction_id,
                                                       uint16_t start_address,
                                                       uint16_t quantity,
                                                       uint8_t unit_id = 0x01);

  // 0x03 读: MW区 quantity: 字数
  static ModbusTcpFrame
  BuildReadHoldingRegistersRequest(uint16_t transaction_id,
                                   uint16_t start_address, uint16_t quantity,
                                   uint8_t unit_id = 0x01);
  // 0x04 读: IW区 quantity: 字数
  static ModbusTcpFrame BuildReadInputRegistersRequest(uint16_t transaction_id,
                                                       uint16_t start_address,
                                                       uint16_t quantity,
                                                       uint8_t unit_id = 0x01);
  // 0x05 写: Q 区 单个
  static ModbusTcpFrame BuildWriteSingleCoilRequest(uint16_t transaction_id,
                                                    uint16_t address,
                                                    bool is_true,
                                                    uint8_t unit_id = 0x01);
  // 0x06 写: MW区 单个
  static ModbusTcpFrame BuildWriteSingleRegisterRequest(uint16_t transaction_id,
                                                        uint16_t address,
                                                        uint16_t value,
                                                        uint8_t unit_id = 0x01);
  // 0x0f 写: Q 区 quantity: 位数
  static ModbusTcpFrame BuildWriteMultipleCoilsRequest(
      uint16_t transactionId, uint16_t startAddress, uint16_t quantity,
      std::initializer_list<uint8_t> args, uint8_t unit_id = 0x01);
  // 0x10 写: MW区 quantity: 字数
  static ModbusTcpFrame BuildWriteMultipleRegistersRequest(
      uint16_t transaction_id, uint16_t start_address, uint16_t quantity,
      std::initializer_list<uint8_t> args, uint8_t unit_id = 0x01);

private:
  BuildFrame() = delete;
  ~BuildFrame() = delete;
};
void printFrame(const ModbusTcpFrame &frame);

} // namespace modbustcp
