# 打开一对虚拟端口

```shell
socat -d -d \
  pty,raw,echo=0,link=/tmp/VCOM1,mode=666 \
  pty,raw,echo=0,link=/tmp/VCOM2,mode=666
```
