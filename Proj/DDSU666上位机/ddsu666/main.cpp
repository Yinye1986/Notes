#include "ddsu666.hpp"
#include <csignal>
#include <iostream>
#include <cstdlib> // 引入 cstdlib 来支持 system() 函数

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// 用于优雅退出的标志
volatile sig_atomic_t keep_running = 1;

// Linux/Unix 信号处理函数
void signal_handler(int sig) {
    keep_running = 0;
}

#ifdef _WIN32
// Windows 控制台事件处理函数
BOOL WINAPI console_handler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        keep_running = 0;
        return TRUE; // 信号已被处理
    }
    return FALSE; // 允许默认处理
}
#endif

int main() {
    // 设置信号处理，捕获 Ctrl+C
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(console_handler, TRUE)) {
        std::cerr << "无法设置控制台处理函数" << std::endl;
        return -1;
    }
#else
    std::signal(SIGINT, signal_handler);
#endif

    // 实例化 Modbus 读取器，注意：Linux 下 COM3 应该改为 /dev/ttyUSB0 或类似设备
    // 假设在 Linux 下使用 /dev/ttyUSB0
#ifdef _WIN32
    DDSU666Reader reader("COM3", 9600, 'N', 8, 1, 11);
#else
    DDSU666Reader reader("/dev/ttyUSB0", 9600, 'N', 8, 1, 11);
#endif

    if (!reader.connect()) {
        std::cerr << "连接失败" << std::endl;
        return -1;
    }

    // 初始清屏，使界面整洁
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J" << "\033[H";
#endif

    std::cout << "成功连接到设备" << std::endl;
    std::cout << "开始读取... (按 Ctrl+C 退出)" << std::endl;

    DDSU666Data data;

    while (keep_running) {
        if (reader.readData(data)) {

            // 实时刷新：清屏与定位光标
#ifdef _WIN32
            // Windows 下清屏
            system("cls");
#else
            // Linux/macOS 下使用 ANSI 序列（清屏 + 光标归位）
            std::cout << "\033[2J" << "\033[H";
#endif

            std::cout << "--- DDSU666 实时数据 ---\n";
            std::cout << "电压: " << data.voltage << " V" << std::endl;
            std::cout << "电流: " << data.current << " A" << std::endl;
            std::cout << "有功功率: " << data.activePower << " W" << std::endl;
            std::cout << "无功功率: " << data.reactivePower << " var" << std::endl;
            std::cout << "视在功率: " << data.apparentPower << " VA" << std::endl;
            std::cout << "功率因数: " << data.powerFactor << std::endl;
            std::cout << "频率: " << data.frequency << " Hz" << std::endl;
            std::cout << "总有功电能: " << data.energy << " kWh" << std::endl;
            std::cout << "------------------------" << std::endl;
            std::cout << "按 Ctrl+C 退出..." << std::endl; // 始终显示退出提示
        } else {
            // 注意：错误信息会随着下次清屏被清除，但能即时显示
            std::cerr << "读取数据失败" << std::endl;
        }

        // 跨平台延时 2 秒
#ifdef _WIN32
        Sleep(2000); // Windows使用毫秒
#else
        sleep(2); // Linux使用秒
#endif
    }

    // 程序退出前执行清理工作
    // 确保在退出前再次清屏，显示最终退出信息
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J" << "\033[H";
#endif

    std::cout << "\n收到退出信号，正在关闭连接..." << std::endl;
    reader.disconnect();
    std::cout << "程序已退出。" << std::endl;
    return 0;
}
