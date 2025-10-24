cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD_REQUIRED ON)


project(
  ModbusSlave
  VERSION 1.0
  LANGUAGES C
)

set(INC_DIRS
  inc
)

set(LIB_NAME "mylib")
set(LIB_SOURCES
  src/modbusSlave.c
)

set(EXE_NAME "file_to_run")
set(EXE_SOURCE
  src/main.c
)

add_library(${LIB_NAME} STATIC ${LIB_SOURCES})
target_include_directories(${LIB_NAME} PUBLIC ${INC_DIRS}) # PUBLIC: 使依赖mylib的目标集成这里的头文件目录
target_compile_features(${LIB_NAME} PRIVATE c_std_17)
set_property(TARGET ${LIB_NAME} PROPERTY C_STANDARD_REQUIRED ON)
target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic)


add_executable(${EXE_NAME} ${EXE_SOURCE})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})
target_compile_features(${EXE_NAME} PRIVATE c_std_17)
set_property(TARGET ${EXE_NAME} PROPERTY C_STANDARD_REQUIRED ON)
target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -pedantic -g)
