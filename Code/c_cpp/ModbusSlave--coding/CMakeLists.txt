cmake_minimum_required(VERSION 4.1.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD_REQUIRED ON)

project(
  ModbusSlave
  VERSION 1.0
  LANGUAGES C
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) # 静态库 (*.a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)  # 动态库 (*.so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)  # 可执行文件

set(INC_DIRS
  inc
)

set(LIB_STATIC "mystaticlib")
set(LIB_SHARED "mysharedlib")
set(LIB_SOURCES
  src/over_tcp.c
)

add_library(${LIB_STATIC} STATIC ${LIB_SOURCES})
target_include_directories(${LIB_STATIC} PUBLIC ${INC_DIRS})
target_compile_features(${LIB_STATIC} PRIVATE c_std_17)
set_property(TARGET ${LIB_STATIC} PROPERTY C_STANDARD_REQUIRED ON)
target_compile_options(${LIB_STATIC} PRIVATE -Wall -Wextra -pedantic)

add_library(${LIB_SHARED} SHARED ${LIB_SOURCES})
target_include_directories(${LIB_SHARED} PUBLIC ${INC_DIRS})
target_compile_features(${LIB_SHARED} PRIVATE c_std_17)
set_property(TARGET ${LIB_SHARED} PROPERTY C_STANDARD_REQUIRED ON)
target_compile_options(${LIB_SHARED} PRIVATE -Wall -Wextra -pedantic)

set(EXE_NAME "file_to_run")
set(EXE_SOURCE
  src/main.c
)

add_executable(${EXE_NAME} ${EXE_SOURCE})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_STATIC})
target_compile_features(${EXE_NAME} PRIVATE c_std_17)
set_property(TARGET ${EXE_NAME} PROPERTY C_STANDARD_REQUIRED ON)
target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -pedantic -g)
