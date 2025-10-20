from pymodbus.client import ModbusSerialClient
from pymodbus.exceptions import ModbusIOException  # 导入异常类
from pymodbus.pdu.diag_message import GetClearModbusPlusResponse
from pymodbus.pdu.diag_message import GetClearModbusPlusRequest
import os
import time

from pymodbus.client import ModbusSerialClient
from pymodbus.exceptions import ModbusIOException
import os
import time
import logging  # 1. 导入logging模块

logging.basicConfig()
log = logging.getLogger()
log.setLevel(logging.DEBUG)# --- 配置参数 ---

if os.name == 'posix':
    PORT = "/dev/ttyUSB0"
elif os.name == 'nt':
    PORT = "COM3"

PARITY = "N"
STOPBITS = 1
BYTESIZE = 8
TIMEOUT = 0.1  # 扫描时可以适当缩短超时，比如0.2，可以加快扫描速度

BAUD_TO_TEST = [9600]
SLAVE_TO_TEST = range(65, 248)

# --- 寄存器地址 ---
HUM = 0x0000 # 扫描时只读一个寄存器即可确认设备存在
TEM = 0x0001

# --- 全局变量，用于保存扫描结果 ---
FOUND_BAUD = None
FOUND_SLAVE = None

def scan():
    """
    扫描Modbus总线，查找有效的波特率和从站地址。
    找到第一个可用设备后，将其参数存入全局变量并立即返回。
    """
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
            print(f">>>> 串口 {PORT} 打开失败")
            continue  # 尝试下一个波特率

        for slave in SLAVE_TO_TEST:
            print(f">>>> 测试从站地址: {slave:<3}", end="\r")
            try:
                # 尝试读取一个保持寄存器
                result = client.read_holding_registers(
                    address=HUM,
                    count=2,
                    device_id=slave
                )
                # 如果读取成功且没有错误
                if not result.isError():
                    FOUND_BAUD = baud
                    FOUND_SLAVE = slave
                    print(f"\n>>>> 成功找到设备 - 波特率: {baud}, 从站地址: {slave}")
                    client.close()
                    return  # 找到后立刻退出函数
            except ModbusIOException:
                # 捕获IO异常（例如超时收不到回应），这在扫描时是正常现象
                # 不需要做任何事，直接继续循环下一个地址
                pass

        client.close()

    print("\n--- 扫描结束 ---")
    print("未找到任何有效的Modbus设备。")

def main():
    """主程序"""
    scan() # 执行扫描

    # 检查扫描是否成功
    if FOUND_BAUD is None or FOUND_SLAVE is None:
        print("无法进行数据读取，因为未找到设备。")
        return # 退出程序

    print("\n--- 使用找到的参数开始读取数据 ---")
    print(f"参数: Port={PORT}, Baudrate={FOUND_BAUD}, Slave ID={FOUND_SLAVE}")

    # 使用扫描到的参数创建新的客户端
    client = ModbusSerialClient(
        port=PORT,
        baudrate=FOUND_BAUD,
        parity=PARITY,
        stopbits=STOPBITS,
        bytesize=BYTESIZE,
        timeout=1,
    )

    if not client.connect():
        print("串口连接失败。")
        return

    try:
        while True:
            # 读取湿度和温度（共2个寄存器）
            result = client.read_holding_registers(
                address=TEM, # 从湿度的地址开始读
                count=2,
                device_id=FOUND_SLAVE
            )

            if not result.isError() and result.registers and len(result.registers) == 2:
                # 传感器数据是原始值的10倍
                raw_humidity = result.registers[0]
                raw_temperature = result.registers[1]

                actual_humidity = raw_humidity / 10.0
                actual_temperature = raw_temperature / 10.0
                # print(raw_temperature, "\n",raw_humidity, "\n")
                print(f"温度: {actual_temperature:.1f}°C, 湿度: {actual_humidity:.1f}%RH", end="\r")
            else:
                print("Modbus 读取错误，请检查设备连接...")

            time.sleep(2)
    except KeyboardInterrupt:
        print("\n程序被用户中断。")
    finally:
        client.close()
        print("串口已关闭。")


if __name__ == "__main__":
   main()
