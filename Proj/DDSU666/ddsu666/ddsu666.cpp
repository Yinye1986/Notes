#include "ddsu666.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>

DDSU666Reader::DDSU666Reader(const char *device, int baud, char parity,
                             int dataBit, int stopBit, int slaveId)
    : ctx(nullptr), device(device), baud(baud), parity(parity),
      dataBit(dataBit), stopBit(stopBit), slaveId(slaveId), connected(false) {}
DDSU666Reader::~DDSU666Reader() { disconnect(); }

bool DDSU666Reader::connect() {
  ctx = modbus_new_rtu(device, baud, parity, dataBit, stopBit);
  if (ctx == nullptr) {
    std::cerr << "无法创建 libmodbus context: " << modbus_strerror(errno)
              << std::endl;
    return false;
  }
  modbus_set_slave(ctx, slaveId);
  if (modbus_connect(ctx) == -1) {
    std::cerr << "连接到 " << device << " 失败: " << modbus_strerror(errno)
              << std::endl;
    modbus_free(ctx);
    ctx = nullptr;
    return false;
  }
  modbus_set_response_timeout(ctx, 0, 500000); // 0.5秒

  connected = true;
  return true;
}

void DDSU666Reader::disconnect() {
  if (ctx != nullptr) {
    if (connected) {
      modbus_close(ctx);
    }
    modbus_free(ctx);
    ctx = nullptr;
    connected = false;
  }
}

bool DDSU666Reader::isConnected() const { return connected; }

bool DDSU666Reader::readFloatRegister(int regAddr, float &value) {
  if (!connected || ctx == nullptr) {
    return false;
  }

  uint16_t tabReg[2];
  if (modbus_read_registers(ctx, regAddr, 2, tabReg) == 2) {
    value = modbus_get_float_abcd(tabReg);
    return true;
  } else {
    std::cerr << "读取寄存器 (0x" << std::hex << regAddr << std::dec
              << ") 失败: " << modbus_strerror(errno) << std::endl;
    return false;
  }
}

// 读取所有电表数据
bool DDSU666Reader::readData(DDSU666Data &data) {
  if (!connected || ctx == nullptr) {
    return false;
  }

  // 清空接收缓冲区
  modbus_flush(ctx);

  // 读取各项数据
  bool success = true;

  success &= readFloatRegister(VOLTAGE_REG, data.voltage);
  success &= readFloatRegister(CURRENT_REG, data.current);
  success &= readFloatRegister(ACTIVE_POWER_REG, data.activePower);
  success &= readFloatRegister(REACTIVE_POWER_REG, data.reactivePower);
  success &= readFloatRegister(APPARENT_POWER_REG, data.apparentPower);
  success &= readFloatRegister(POWER_FACTOR_REG, data.powerFactor);
  success &= readFloatRegister(FREQUENCY_REG, data.frequency);
  success &= readFloatRegister(ENERGY_REG, data.energy);

  return success;
}
