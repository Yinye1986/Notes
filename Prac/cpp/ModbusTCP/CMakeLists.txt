# 1. 最小版本要求
# 强制要求 CMake 版本在 3.10 及以上。
# 现代 CMake 特性（如 target_link_libraries 的 PUBLIC/PRIVATE）需要较高的版本支持。
cmake_minimum_required(VERSION 3.10)

# 2. 项目名称
# project() 函数必须是第一个非注释的命令。
# 它定义了项目名 (ModbusTCP_Project) 和项目所使用的语言 (CXX/C++)。
# 这个名称会被用于一些默认变量（如 ${PROJECT_NAME}）。
project(ModbusTCP_Project CXX)

# 3. 设置通用编译选项

set(CMAKE_CXX_STANDARD 17) # 告诉编译器使用 C++17 语言特性。
# 如果编译器不支持 C++17，则停止构建，而不是回退到旧标准。
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 开启位置无关代码 (Position-Independent Code, PIC)
# 这是一个非常重要的全局设置。
# **原因：** 动态库（.so）要求其内部代码是 PIC，以便操作系统能将库加载到内存的任意位置。
# **规范：** 推荐在项目级别打开此项，以简化动态库的创建。
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 4. 包含头文件目录
# **规范：** 推荐使用 include_directories 或 target_include_directories。
# **include_directories(inc)** 告诉所有后续定义的目标 (mylib 和 example_app)
# 在编译时都去 'inc' 目录查找头文件 (如 modbusTCP.hpp)。
# 这是为了满足 `#include "modbusTCP.hpp"` 在预处理阶段的需求。
include_directories(inc)

# -----------------------------------------------------
# 5. 构建库目标 (libmylib.a 或 libmylib.so)
# -----------------------------------------------------

# 定义库的目标名称
# **规范：** 在 CMake 中定义的目标名称 (mylib) 不必包含 'lib' 前缀或 '.a/.so' 后缀。
# CMake 会自动添加这些前缀和后缀，例如 mylib 目标会生成 libmylib.a 或 libmylib.so。
set(LIB_NAME mylib)
set(LIB_SOURCES src/modbusTCP.cpp)

# -------------------
# 5A. 静态库示例 (libmylib.a)
# **add_library(mylib STATIC ...)**：告诉 CMake 构建一个静态库。
# add_library(${LIB_NAME} STATIC ${LIB_SOURCES})
# -------------------

# 5B. 动态库示例 (libmylib.so) - 推荐用于模块化
# **add_library(mylib SHARED ...)**：告诉 CMake 构建一个动态库 (Shared Object)。
# **原因：** SHARED 库便于系统资源共享和未来升级，是现代 Linux 开发的首选。
add_library(${LIB_NAME} SHARED ${LIB_SOURCES})

# -----------------------------------------------------
# 6. 构建可执行文件目标 (example_app)
# -----------------------------------------------------

# 定义可执行文件的目标名称和源文件
set(EXE_NAME example_app)
set(EXE_SOURCE src/example.cpp)

# **add_executable()**：创建最终的可执行文件目标。
add_executable(${EXE_NAME} ${EXE_SOURCE})

# 7. 链接依赖
# **target_link_libraries()**：指定目标 (example_app) 依赖于哪些库。
# **规范：** 这是一个现代 CMake 的核心命令，比直接使用 LINK_LIBRARIES 更好。
# **PRIVATE 关键字：** 意味着 mylib 只是 example_app 内部需要的依赖，
# mylib 的依赖不会被传递给任何链接 example_app 的其他目标。
# **原因：** 链接器将 example_app.o 与 libmylib.so/a 关联起来，完成了链接阶段的工作。
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})
