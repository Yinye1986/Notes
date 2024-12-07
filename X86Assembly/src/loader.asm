[org 0x1000]

dw 0x55aa

mov si, str
call print

mov edi, 0x1000
mov ecx, 0; 起始扇区
mov bl, 1

call readDisk

jmp $

print:
    mov ah, 0x0e ; 在ah寄存器中放入中断服务的子功能号,即可实现指定bios中断调用 此处0x0e是在屏幕上显示指定内容
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

readDisk
