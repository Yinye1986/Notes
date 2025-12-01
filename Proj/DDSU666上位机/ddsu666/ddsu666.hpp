#ifndef DDSU666_H
#define DDSU666_H

#ifdef _WIN32
#include <modbus.h>
#else
#include <modbus/modbus.h>
#endif
#include <cstdint>

struct DDSU666Data {
    float voltage;         
    float current;         
    float activePower;     
    float reactivePower;   
    float apparentPower;   
    float powerFactor;     
    float frequency;       
    float energy;          
};

class DDSU666Reader {
public:
    DDSU666Reader(const char* device, int baud, char parity, int dataBit, int stopBit, int slaveId);
    
    ~DDSU666Reader();
    
    bool connect();
    void disconnect();
    bool readData(DDSU666Data& data);
    bool isConnected() const;
    // 该函数不会修改类的任何成员变量
    // 该函数可以被 const 修饰的对象调用
    // 在函数内部，不能调用非 const 的成员函数


private:
    modbus_t* ctx;         // Modbus上下文
    const char* device;    
    int baud;              
    char parity;           
    int dataBit;           
    int stopBit;           
    int slaveId;           
    
    static const int VOLTAGE_REG = 0x2000;
    static const int CURRENT_REG = 0x2002;
    static const int ACTIVE_POWER_REG = 0x2004;
    static const int REACTIVE_POWER_REG = 0x2006;
    static const int APPARENT_POWER_REG = 0x2008;
    static const int POWER_FACTOR_REG = 0x200A;
    static const int FREQUENCY_REG = 0x200E;
    static const int ENERGY_REG = 0x4000;
    
    bool connected;
    
    bool readFloatRegister(int regAddr, float& value);
};

#endif
