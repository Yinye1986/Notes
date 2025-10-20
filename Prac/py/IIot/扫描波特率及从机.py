# 遍历波特率和从机地址
# 使用需指定一个保持寄存器地址以尝试获取响应
from pymodbus.client import ModbusSerialClient
from pymodbus.exceptions import ModbusIOException
import os
import time

if os.name == 'posix':
    PORT = "/dev/ttyUSB0"
elif os.name == 'nt':
    PORT = "COM3"

PARITY = "N"
STOPBITS = 1
BYTESIZE = 8
TIMEOUT = 0.1

BAUD_TO_TEST =[
    9600,
    19200,
    38400,
    4800,
    2400,
    1200,
]
SLAVE_TO_TEST = range(65, 248)

HUM = 0x0000
TEM = 0x0001

FOUND_BAUD = None
FOUND_SLAVE = None

def scan():
    global FOUND_BAUD, FOUND_SLAVE
    print("--- 扫描开始 ---")

    for baud in BAUD_TO_TEST:
        print(f"\n>>>> 测试波特率: {baud}...")
        client = ModbusSerialClient(
            port=PORT,
            baudrate=baud,
            parity=PARITY,
            stopbits=STOPBITS,
            bytesize=BYTESIZE,
            timeout=TIMEOUT,
        )
        if not client.connect():
            print(f"!!!! 串口{PORT}打开失败")
            continue

        for slave in SLAVE_TO_TEST:
            print(f">> 测试从站地址: {slave:<3}", end="\r")
            try:
                result = client.read_holding_registers(
                    address=HUM,
                    count=2,
                    device_id=slave
                )
                if not result.isError():
                    FOUND_BAUD = baud
                    FOUND_SLAVE = slave
                    print(f"\n>> 成功找到设备 - 波特率: {baud}, 从站地址: {slave}")
                    client.close()
                    return
            except ModbusIOException:
                pass
        client.close()

    print("\n--- 扫描结束 ---")
    print("未找到任何有效的Modbus设备")

def main():
    scan()
    if FOUND_BAUD is None or FOUND_SLAVE is None:
        print("未找到设备, 无法继续操作")
        return
    # ...
    # client = ModbusSerialClient(
    #     port=PORT,
    #     baudrate=FOUND_BAUD,
    #     parity=PARITY,
    #     stopbits=STOPBITS,
    #     bytesize=BYTESIZE,
    #     timeout=1,
    # )

    # if not client.connect():
    #     print("串口连接失败")
    #     return

if __name__ == "__main__":
   main()
