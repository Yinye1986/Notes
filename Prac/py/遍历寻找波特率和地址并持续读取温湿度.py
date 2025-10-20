# ModbusHostScanBaudAndSlaveid
from pymodbus.client import ModbusSerialClient
import os
import time

if os.name == 'posix':
    PORT = "/dev/ttyUSB0"
elif os.name == 'nt':
    PORT = "COM3"
# BAUD = 9600
PARITY = "N"
STOPBITS = 1
BYTESIZE = 8
TIMEOUT = 0.5

BAUD_TO_TEST = [
    9600,
    19200,
    38400,
    4800,
    2400,
    1200,
]

SLAVE_TO_TEST = range(1, 248)

TEM = 0x0001;
HUM = 0x0000;

def scan():
    global FOUND_BAUD
    global FOUND_SLAVE
    print("--- 扫描开始 ---")
    for baud in BAUD_TO_TEST:
        print(f"\n>>>> 测试BAUD: {baud}...")
        client = ModbusSerialClient(
            port=PORT,
            baudrate=baud,
            parity=PARITY,
            stopbits=STOPBITS,
            bytesize=BYTESIZE,
            timeout=TIMEOUT,
        )
        if not client.connect():
            print(f"\n>>>> 串口打开失败")
            continue
        for slave in SLAVE_TO_TEST:
            print(f">>>> 测试ADDR: {slave:<3}", end="\r")
            result = client.read_holding_registers(
                    address=HUM,
                    count=1,
                    device_id=slave
            )
            if not result.isError() and result.registers is not None:
                if len(result.registers) == 1:
                    FOUND_BAUD = baud
                    FOUND_SLAVE = slave
                    print(f"\n>>>> BAUD: {baud}, SLAVE: {slave}")
            pass
        client.close()
    print("\n--- 扫描结束 ---")
    print("未找到有效参数")
    return

if __name__ == "__main__":
        scan()
        if os.name == 'posix':
            os.system('clear')
        elif os.name == 'nt':
            os.system('cls')
        client = ModbusSerialClient(
            port=PORT,
            baudrate=FOUND_BAUD,
            parity=PARITY,
            stopbits=STOPBITS,
            bytesize=BYTESIZE,
            timeout=1,
        )
        if client.connect():
            while 1:
                result = client.read_holding_registers(
                    address=HUM,
                    count=2,
                    device_id=FOUND_SLAVE
                )
                if not result.isError() and len(result.registers) == 2:
                    # 原始值
                    raw_humidity = result.registers[0]
                    raw_temperature = result.registers[1]
                    # 真实值
                    actual_humidity = raw_humidity / 10.0
                    actual_temperature = raw_temperature / 10.0
                    print(f"TEM: {actual_temperature:<5}\nHUM: {actual_humidity}", end="\r")
                else:
                    print("Modbus 读取错误")
                time.sleep(2)
        else:
            print("串口连接失败")
            client.close()
