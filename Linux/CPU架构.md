# 常见CPU架构

本文记号：}}}表示不会

## 精简指令集 （Reduced Instruction Set Computer, RISC）：

- arm的arm
- IBM的power architecture
- oracle的SPARC

### 优劣

- 能耗低
-

## 复杂指令集（Complex Instruction Set Computer, CISC）：

最早由intel开发的8086奠定了x86架构这个名字

根据CPU内存寻址能力，细分为32位， 64位……

### 影响

- 内存大小：由于每个内存地址通常对应一个字节（Byte）的数据，所以32位CPU能够访问的最大内存空间就是2^32个字节。按照计算机存储单位的换算规则，1024B=1KB，1024KB=1MB，1024MB=1GB，所以2^32B = 2^32/1024/1024/1024 = 4GB。}}}

- 先进的指令集有更高的效率，更好的多媒体性能，更好的虚拟化性能

