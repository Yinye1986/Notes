cmake_minimum_required (VERSION 3.5)

project (LinkedList)

aux_source_directory(./src SRCS)

add_executable(Demo ${SRCS})
