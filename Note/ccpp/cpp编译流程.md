# 生成纯 c文件(文本)
g++ -E src/example.cpp -I inc -o example.i

使用编译器`gcc`将cpp预处理为.i
但是项目编译中通常不会单独进行这一步

# 生成汇编源码(汇编)
g++ -S example.i -o example.s

使用`cc1plus`将.i文件转化成汇编代码

# 生成目标文件(机器码)
g++ -c example.s -o example.o

使用汇编器`as`将.i文件转化成可以执行的目标代码.o

# 生成可执行文件(二进制)
g++ example.o modbusTCP.o -o exefile

使用链接器`ld`将.o文件链接为可执行文件
