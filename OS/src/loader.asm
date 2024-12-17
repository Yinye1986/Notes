[org 0x1000]

dw 0x55aa; 魔数, 用于判断错误
; loader读到0x1000开头两个字节为55aa

xchg bx, bx

mov si, loading

call print

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

loading:
    db "Loading Onix...", 10, 13, 0; \n\r
