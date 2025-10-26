#include "modbus_tcp_frame.hpp"

#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <iostream>

// ModbusTcpFrame结构体成员函数
std::vector<uint8_t> modbustcp::ModbusTcpFrame::toBytes() const {
  std::vector<uint8_t> bytes;
  bytes.reserve(8 +
                this->data_.size()); // MBAP头(7字节) + function_code_ + 数据
  bytes.push_back((this->transaction_id_ >> 8) & 0xFF);
  bytes.push_back(this->transaction_id_ & 0xFF);
  bytes.push_back((this->protocol_id_ >> 8) & 0xFF);
  bytes.push_back(this->protocol_id_ & 0xFF);
  bytes.push_back((this->length_ >> 8) & 0xFF);
  bytes.push_back(this->length_ & 0xFF);
  bytes.push_back(this->unit_id_);
  bytes.push_back(this->function_code_);
  bytes.insert(bytes.end(), this->data_.begin(), this->data_.end());
  return bytes;
}
modbustcp::ModbusTcpFrame modbustcp::ModbusTcpFrame::fromBytes(const std::vector<uint8_t> &bytes) {
  ModbusTcpFrame frame;
  if (bytes.size() < 8) {
    return frame;
  }
  frame.data_.reserve(bytes.size() - 8);
  frame.transaction_id_ = (bytes[0] << 8) | bytes[1];
  frame.protocol_id_ = (bytes[2] << 8) | bytes[3];
  frame.length_ = (bytes[4] << 8) | bytes[5];
  frame.unit_id_ = bytes[6];
  frame.function_code_ = bytes[7];
  if (bytes.size() > 8) {
    frame.data_.assign(bytes.begin() + 8, bytes.end());
  }
  return frame;
}
modbustcp::ModbusTcpFrame::ModbusTcpFrame(uint16_t transaction_id, uint8_t unit_id,
                               uint8_t function_code)
    : transaction_id_(transaction_id), protocol_id_(0x0000), unit_id_(unit_id),
      function_code_(function_code) {}

// BuildFrame类成员函数实现
modbustcp::ModbusTcpFrame modbustcp::BuildFrame::BuildReadCoilRequest(uint16_t transaction_id,
                                                uint16_t start_address,
                                                uint16_t quantity,
                                                uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x01);
  frame.data_.reserve(4);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame modbustcp::BuildFrame::BuildReadDiscreteInputsRequset(uint16_t transaction_id,
                                           uint16_t start_address,
                                           uint16_t quantity, uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x02);
  frame.data_.reserve(4);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame modbustcp::BuildFrame::BuildReadHoldingRegistersRequest(
    uint16_t transaction_id, uint16_t start_address, uint16_t quantity,
    uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x03);
  frame.data_.reserve(4);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame
modbustcp::BuildFrame::BuildReadInputRegistersRequest(uint16_t transaction_id,
                                           uint16_t start_address,
                                           uint16_t quantity, uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x04);
  frame.data_.reserve(4);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame modbustcp::BuildFrame::BuildWriteSingleCoilRequest(uint16_t transaction_id,
                                                       uint16_t address,
                                                       bool is_true,
                                                       uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x05);
  frame.data_.reserve(4);
  frame.data_.push_back((address >> 8) & 0xFF);
  frame.data_.push_back(address & 0xFF);
  if (is_true == 1) {
    frame.data_.push_back(0xFF);
    frame.data_.push_back(0x00);
  } else {
    frame.data_.push_back(0x00);
    frame.data_.push_back(0x00);
  }
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame
modbustcp::BuildFrame::BuildWriteSingleRegisterRequest(uint16_t transaction_id,
                                            uint16_t address, uint16_t value,
                                            uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x06);
  frame.data_.reserve(4);
  frame.data_.push_back((address >> 8) & 0xFF);
  frame.data_.push_back(address & 0xFF);
  frame.data_.push_back((value >> 8) & 0xFF);
  frame.data_.push_back(value & 0xFF);
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame modbustcp::BuildFrame::BuildWriteMultipleCoilsRequest(
    uint16_t transaction_id, uint16_t start_address, uint16_t quantity,
    std::initializer_list<uint8_t> args, uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x0F);
  frame.data_.reserve(4 + args.size());
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.data_.insert(frame.data_.end(), args.begin(), args.end());
  frame.length_ = 2 + frame.data_.size();
  return frame;
}
modbustcp::ModbusTcpFrame modbustcp::BuildFrame::BuildWriteMultipleRegistersRequest(
    uint16_t transaction_id, uint16_t start_address, uint16_t quantity,
    std::initializer_list<uint8_t> args, uint8_t unit_id) {
  ModbusTcpFrame frame(transaction_id, unit_id, 0x10);
  frame.data_.reserve(5 + args.size());
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.data_.push_back(quantity * 2);
  frame.data_.insert(frame.data_.end(), args.begin(), args.end());
  frame.length_ = 2 + frame.data_.size();
  return frame;
}

void modbustcp::printFrame(const ModbusTcpFrame &frame) {
  if (!frame.data_.empty()) {
    std::vector<uint8_t> vec = frame.toBytes();
    for (size_t i = 0; i < vec.size(); ++i) {
      if (i > 0) {
        std::cout << " ";
      }
      std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << static_cast<int>(vec[i]);
    }
    std::cout << std::dec << std::endl;
  }
}
