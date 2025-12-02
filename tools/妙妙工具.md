# 内建命令type

可以识别:

- 内建命令: type是一个内建命令
- 外部命令: sway是/usr/bin/sway
- 函数    : 函数定义
- 别名    : 别名定义
- sh脚本  : xxx.sh是xxx.sh
- ...

# 内建命令command

command ls 执行 ls 程序, 即使存在 ls 别名

command -v ls 返回路径或定义

# man-db提供whatis

分析 Man Page 数据库, 返回命令是干嘛的

可以识别: 数据库中有的

# which提供which

which sway: /usr/bin/sway

# util-linux提供whereis

Binary (可执行文件)：命令的主体程序，通常位于 $PATH 中的目录。

Source (源码文件)：命令的源代码文件（如果已安装）。

Manual (手册页)：命令的帮助文档，即 man 手册文件。

# tealdeer提供tldr(Rust实现, 可选其他)

显示来自 tldr-pages 项目提供的命令常用示例

# file提供file

返回 文件/目录 类型

# pacman -Qo /usr/bin/sway

查找哪个软件包提供的bin

# sudo pacman -Fy

查找哪个软件包能提供
