# 本项目是基于libmodbus实现的DDSU666智能电表上位机
## libmodbus在windows下的编译
```shell
pacman -S automake libtool autoconf-wrapper mingw-w64-x86_64-gcc
cd /D/libmodbus
./autogen.sh
./configure
x86_64-w64-mingw32-g++ -std=c++11 -O2 -Ilibmodbus_win/include \
    -Llibmodbus_win -llibmodbus \
    main.cpp ddsu666.cpp \
    -o ddsu666_reader.exe \
    -static-libgcc -static-libstdc++
```

## 使用

1. `ddsu666.exe` - 编译生成的可执行文件
2. `libmodbus-5.dll` - libmodbus 动态库

确保libmodbus.dll与可执行文件位于同一目录
或将该DLL添加到系统的PATH环境变量中
