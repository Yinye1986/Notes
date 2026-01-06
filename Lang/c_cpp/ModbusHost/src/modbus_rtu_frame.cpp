#include "modbus_rtu_frame.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

uint16_t modbusrtu::ModbusRtuFrame::getCrc16() const {
  uint16_t crc = 0xFFFF;
  const uint16_t poly = 0xA001;
  std::vector<uint8_t> frame;
  frame.reserve(2 + this->data_.size());
  frame.push_back(slave_id_);
  frame.push_back(function_code_);
  frame.insert(frame.end(), data_.begin(), data_.end());
  for (uint8_t i : frame) {
    crc ^= static_cast<uint16_t>(i);

    for (int j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= poly;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
};

std::vector<uint8_t> modbusrtu::ModbusRtuFrame::toBytes() const {
  std::vector<uint8_t> bytes;
  bytes.reserve(2 + this->data_.size());
  bytes.push_back(this->slave_id_);
  bytes.push_back(this->function_code_);
  bytes.insert(bytes.end(), this->data_.begin(), this->data_.end());
  return bytes;
}

modbusrtu::ModbusRtuFrame
modbusrtu::ModbusRtuFrame::fromBytes(const std::vector<uint8_t> &bytes) {
  ModbusRtuFrame frame;
  frame.data_.reserve(bytes.size() - 2);
  frame.slave_id_ = bytes[0];
  frame.function_code_ = bytes[1];
  frame.data_.assign(bytes.begin() + 2, bytes.end());
  return frame;
}

modbusrtu::ModbusRtuFrame::ModbusRtuFrame(uint8_t slave_id,
                                          uint8_t function_code)
    : slave_id_(slave_id), function_code_(function_code) {}

// 01
modbusrtu::ModbusRtuFrame modbusrtu::BuildFrame::BuildReadCoilRequest(
    uint8_t slave_id, uint16_t start_address, uint16_t quantity) {

  ModbusRtuFrame frame(slave_id, 0x01);
  frame.data_.reserve(6);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame modbusrtu::BuildFrame::BuildReadDiscreteInputsRequset(
    uint8_t slave_id, uint16_t start_address, uint16_t quantity) {
  ModbusRtuFrame frame(slave_id, 0x02);
  frame.data_.reserve(6);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame
modbusrtu::BuildFrame::BuildReadHoldingRegistersRequest(uint8_t slave_id,
                                                        uint16_t start_address,
                                                        uint16_t quantity) {
  ModbusRtuFrame frame(slave_id, 0x03);
  frame.data_.reserve(6);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame modbusrtu::BuildFrame::BuildReadInputRegistersRequest(
    uint8_t slave_id, uint16_t start_address, uint16_t quantity) {
  ModbusRtuFrame frame(slave_id, 0x04);
  frame.data_.reserve(6);
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame modbusrtu::BuildFrame::BuildWriteSingleCoilRequest(
    uint8_t slave_id, uint16_t address, bool is_true) {
  ModbusRtuFrame frame(slave_id, 0x05);
  frame.data_.reserve(6);
  frame.data_.push_back((address >> 8) & 0xFF);
  frame.data_.push_back(address & 0xFF);
  if (is_true == 1) {
    frame.data_.push_back(0xFF);
    frame.data_.push_back(0x00);
  } else {
    frame.data_.push_back(0x00);
    frame.data_.push_back(0x00);
  }
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame
modbusrtu::BuildFrame::BuildWriteSingleRegisterRequest(uint8_t slave_id,
                                                       uint16_t address,
                                                       uint16_t value) {
  ModbusRtuFrame frame(slave_id, 0x06);
  frame.data_.reserve(6);
  frame.data_.push_back((address >> 8) & 0xFF);
  frame.data_.push_back(address & 0xFF);
  frame.data_.push_back((value >> 8) & 0xFF);
  frame.data_.push_back(value & 0xFF);
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame modbusrtu::BuildFrame::BuildWriteMultipleCoilsRequest(
    uint8_t slave_id, uint16_t start_address, uint16_t quantity,
    std::initializer_list<uint8_t> args) {
  ModbusRtuFrame frame(slave_id, 0x0f);
  frame.data_.reserve(6 + args.size());
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.data_.insert(frame.data_.end(), args.begin(), args.end());
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
modbusrtu::ModbusRtuFrame
modbusrtu::BuildFrame::BuildWriteMultipleRegistersRequest(
    uint8_t slave_id, uint16_t start_address, uint16_t quantity,
    std::initializer_list<uint8_t> args) {
  ModbusRtuFrame frame(slave_id, 0x10);
  frame.data_.reserve(7 + args.size());
  frame.data_.push_back((start_address >> 8) & 0xFF);
  frame.data_.push_back(start_address & 0xFF);
  frame.data_.push_back((quantity >> 8) & 0xFF);
  frame.data_.push_back(quantity & 0xFF);
  frame.data_.push_back(quantity * 2);
  frame.data_.insert(frame.data_.end(), args.begin(), args.end());
  uint16_t crc = frame.getCrc16();
  frame.data_.push_back(crc & 0xFF);
  frame.data_.push_back((crc >> 8) & 0xFF);
  return frame;
}
void modbusrtu::printFrame(const modbusrtu::ModbusRtuFrame &frame) {
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
