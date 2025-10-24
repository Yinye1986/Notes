# 最小版本要求
cmake_minimum_required(VERSION 3.10) # 强制要求 CMake 版本在 3.10 及以上
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置通用编译选项
set(CMAKE_CXX_STANDARD 23) # 告诉编译器使用 C++17 语言特性。
set(CMAKE_CXX_STANDARD_REQUIRED True) # 如不支持 C++17, 则停止构建, 而不是回退到旧标准
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # 开启Position-Independent Code

project(ModbusHost CXX)

set(INC_DIRS
  inc
)

set(LIB_NAME mylib)
set(LIB_SOURCES src/modbusTCP.cpp)

set(EXE_NAME filename_to_run)
set(EXE_SOURCE src/example.cpp)

add_library(${LIB_NAME} STATIC ${LIB_SOURCES})
target_include_directories(${LIB_NAME} PUBLIC ${INC_DIRS})

add_executable(${EXE_NAME} ${EXE_SOURCE})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})
