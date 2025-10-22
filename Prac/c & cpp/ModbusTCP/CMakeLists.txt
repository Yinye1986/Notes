# 最小版本要求
cmake_minimum_required(VERSION 3.10) # 强制要求 CMake 版本在 3.10 及以上

# 项目名称及语言指定
project(ModbusTCP_Project CXX)

# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置通用编译选项

set(CMAKE_CXX_STANDARD 17) # 告诉编译器使用 C++17 语言特性。
set(CMAKE_CXX_STANDARD_REQUIRED True) # 如不支持 C++17, 则停止构建, 而不是回退到旧标准
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # 开启Position-Independent Code


# 包含头文件目录
# target_include_directories()
include_directories(inc)

# 构建库目标
set(LIB_NAME mylib)
set(LIB_SOURCES src/modbusTCP.cpp)
add_library(${LIB_NAME} STATIC ${LIB_SOURCES}) # 保留此项以生成静态库
# add_library(${LIB_NAME} SHARED ${LIB_SOURCES}) # 保留此项以生成动态库

# 构建可执行文件目标
set(EXE_NAME filename_to_run)
set(EXE_SOURCE src/example.cpp)
add_executable(${EXE_NAME} ${EXE_SOURCE}) # 保留此项以生成可执行文件

# 7. 链接依赖
# **target_link_libraries()**：指定目标 (example_app) 依赖于哪些库。
# **规范：** 这是一个现代 CMake 的核心命令，比直接使用 LINK_LIBRARIES 更好。
# **PRIVATE 关键字：** 意味着 mylib 只是 example_app 内部需要的依赖，
# mylib 的依赖不会被传递给任何链接 example_app 的其他目标。
# **原因：** 链接器将 example_app.o 与 libmylib.so/a 关联起来，完成了链接阶段的工作。
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})
