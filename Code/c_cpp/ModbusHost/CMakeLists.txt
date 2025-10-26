# 最小版本要求
cmake_minimum_required(VERSION 4.1.1) # 强制要求 CMake 版本在 3.10 及以上
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成compile_commands.json
# 设置通用编译选项
set(CMAKE_CXX_STANDARD 23) # 告诉编译器使用 C++23 语言特性。
set(CMAKE_CXX_STANDARD_REQUIRED True) # 如不支持 C++23, 则停止构建, 而不是回退到旧标准
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # 开启Position-Independent Code

project(ModbusHost CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) # 静态库 (*.a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)  # 动态库 (*.so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)  # 可执行文件

set(INC_DIRS
  inc
)

set(LIB_STATIC mystaticlib)
set(LIB_SHARED mysharedlib)
set(LIB_SOURCES src/modbus_tcp_frame.cpp src/modbus_tcp_client.cpp)

add_library(${LIB_STATIC} STATIC ${LIB_SOURCES})
target_include_directories(${LIB_STATIC} PUBLIC ${INC_DIRS})

add_library(${LIB_SHARED} SHARED ${LIB_SOURCES})
target_include_directories(${LIB_SHARED} PUBLIC ${INC_DIRS})

set(EXE_NAME filename_to_run)
set(EXE_SOURCE src/main.cpp)
add_executable(${EXE_NAME} ${EXE_SOURCE})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_STATIC})
