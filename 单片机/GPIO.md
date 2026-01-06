# 推挽输出

P-MOS, N-MOS 配合使用, 支持输出高低电平

writePin(1) P-MOS 开, N-MOS 关, 输出 3.3v
writePin(0) P-MOS 关, N-MOS 开, 输出 0v

# 开漏输出

P-MOS 始终关闭, N-MOS 可选开关, 只能输出高阻态和低电平

writePin(1) P-MOS 关, N-MOS 关, 此时引脚悬空(高阻态)
writePin(0) P-MOS 关, N-MOS 开, 此时外接 3.3v 与 VSS 形成电势差, 导通

注意: 如果外接的是 5v, 需要使用 5v-tolerant 引脚, 否则保护二极管长期导通, 将 5v 引入电源, 导致损坏
