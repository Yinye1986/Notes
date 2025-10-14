#include "ddsu666.h"
#include <iostream>
#include <csignal>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

volatile sig_atomic_t keep_running = 1;
void signal_handler(int sig) {
    keep_running = 0;
}

#ifdef _WIN32
BOOL WINAPI console_handler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        keep_running = 0;
        return TRUE;
    }
    return FALSE;
}
#endif

int main() {
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(console_handler, TRUE)) {
        std::cerr << "无法设置控制台处理函数" << std::endl;
        return -1;
    }
#else
    std::signal(SIGINT, signal_handler);
#endif
    DDSU666Reader reader("COM3", 9600, 'N', 8, 1, 11);
    if (!reader.connect()) {
        std::cerr << "连接失败" << std::endl;
        return -1;
    }
    std::cout << "成功连接到设备" << std::endl;
    std::cout << "开始读取... (按 Ctrl+C 退出)" << std::endl;
    DDSU666Data data;
    while (keep_running) {
        if (reader.readData(data)) {
            std::cout << "电压: " << data.voltage << " V" << std::endl;
            std::cout << "电流: " << data.current << " A" << std::endl;
            std::cout << "有功功率: " << data.activePower << " W" << std::endl;
            std::cout << "无功功率: " << data.reactivePower << " var" << std::endl;
            std::cout << "视在功率: " << data.apparentPower << " VA" << std::endl;
            std::cout << "功率因数: " << data.powerFactor << std::endl;
            std::cout << "频率: " << data.frequency << " Hz" << std::endl;
            std::cout << "总有功电能: " << data.energy << " kWh" << std::endl;
            std::cout << "------------------------" << std::endl;
        } else {
            std::cerr << "读取数据失败" << std::endl;
        }
#ifdef _WIN32
        Sleep(2000); // Windows使用毫秒
#else
        sleep(2);    // Linux使用秒
#endif
    }
    std::cout << "\n收到退出信号，正在关闭连接..." << std::endl;
    reader.disconnect();
    std::cout << "程序已退出。" << std::endl;
    return 0;
}
