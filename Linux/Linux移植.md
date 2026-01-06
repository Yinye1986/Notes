实验记录：从零构建树莓派 4B 嵌入式 Linux 系统

实验环境： Arch Linux (x86_64 宿主机)

目标平台： Raspberry Pi 4 Model B (aarch64)

核心组件： U-Boot, Linux Kernel 6.6.y, BusyBox (Static), Initramfs

# 交叉编译环境准备

宿主机上安装交叉编译所需的工具链

```shell
sudo pacman -S aarch64-linux-gnu-gcc \ # 包含glibc linux-api-headers binutils
    base-devel \ # 包含多种开发工具
    uboot-tools
```


# 构建 BusyBox 根文件系统 (Rootfs)

采用静态编译方案，以摆脱对动态链接库的依赖。

编译 BusyBox:

```shell
# 生成空配置文件
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- allnoconfig
# 在 menuconfig 开启需要的条目; 静态编译, 以及一系列需要的命令(INIT ASH LS CAT MOUNT MKDIR UMOUNT VI ...)
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- menuconfig
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
```

此处有坑 menuconfig 需要宿主机安装 ncurses; 实验环境中有也无效

于是手动修改生成的 .config

构建目录结构：
```shell
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- CONFIG_PREFIX=./rootfs install
mkdir -p rootfs/{etc/init.d,proc,sys,dev,tmp,root}
```

编写核心初始化脚本 (rootfs/etc/init.d/rcS)：

```shell
#!/bin/sh

mount -t proc none /proc
mount -t sysfs none /sys
mount -t tmpfs none /dev
echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s
```

    chmod +x rootfs/etc/init.d/rcS

    建立 Init 链接：
    这一步其实不需要, busybox install的时候以及完成了 bin/busybox 到 sbin/init 的软链接
    以防万一完成 rootfs/bin/busybox 到 rootfs/init 的软链接

    ln -s bin/busybox sbin/init
    ln -s bin/busybox init

    打包 Initramfs (rootfs.cpio.gz)：
    find . | fakeroot cpio -o -H newc | gzip > ../outputs/rootfs.cpio.gz


三、 构建内核与设备树 (Kernel & DTB)

    获取树莓派官方维护内核源码:

    git clone --depth=1 -b rpi-6.6.y https://github.com/raspberrypi/linux.git
    配置与编译:

    cd linux
    make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2711_defconfig
    # 确保开启：CONFIG_BLK_DEV_INITRD=y, CONFIG_SERIAL_AMBA_PL011=y

    make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image dtbs -j$(nproc)

    获取产物:

    cp arch/arm64/boot/Image ../outputs
    cp arch/arm64/boot/dts/broadcom/bcm2711-rpi-4-b.dtb ../outputs


四、 构建引导程序 (U-Boot)

    编译 U-Boot:
    make CROSS_COMPILE=aarch64-linux-gnu- rpi_4_defconfig

    make CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)

    产物:u-boot.bin

    编写引导脚本 (boot.cmd):
    ```boot.cmd
    setenv kernel_addr_r 0x00080000

    setenv fdt_addr_r    0x02e00000
    setenv ramdisk_addr_r 0x03000000

    # 注意：:${filesize} 用于解决 Wrong Ramdisk Image Format 报错
    setenv bootargs "console=ttyAMA0,115200 root=/dev/ram0 rw init=/sbin/init"

    fatload mmc 0:1 ${kernel_addr_r} Image
    fatload mmc 0:1 ${fdt_addr_r} bcm2711-rpi-4-b.dtb
    fatload mmc 0:1 ${ramdisk_addr_r} rootfs.cpio.gz
    booti ${kernel_addr_r} ${ramdisk_addr_r}:${filesize} ${fdt_addr_r}
    ```

    使用 mkconfig 生成二进制脚本

    mkimage -C none -A arm64 -T script -d boot.cmd boot.scr

    获取产物:

    cp u-boot.bin ../outputs
    cp boot.scr ../outputs


五、 SD 卡部署与启动配置

    SD 卡分区:

    分区类型为 W95 FAT32 (LBA)

    mkfs.fat -F 32 /dev/sda1

    下载官方固件:
        start4.elf
        fixup4.dat
        bootcode.bin

    部署文件清单:

        u-boot.bin

        Image

        bcm2711-rpi-4-b.dtb

        boot.scr

        rootfs.cpio.gz

        start4.elf

        fixup4.dat

        bootcode.bin
    将这些文件全部放到 sd 中

    编写 config.txt：

```config.txt
arm_64bit=1
enable_uart=1
kernel=u-boot.bin

```


六、 实验总结与调试心得

    静态链接的重要性：在移植初期使用静态编译的 BusyBox 可以极大地降低因库文件缺失导致的 Kernel Panic。

    Initramfs 格式校验：U-Boot 的 booti 命令对裸 cpio 格式校验严格，在加载时必须显式指定 ${filesize}。

    串口权衡：树莓派 4B 有多个 UART，若 Starting kernel ... 后无输出，通常需在 bootargs 中切换 ttyAMA0 或 ttyS0，并配合 earlycon 调试。

    打包权限：制作 rootfs 镜像时必须使用 fakeroot，否则 root 用户权限无法正确保留。
