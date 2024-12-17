[org 0x1000]

dw 0x55aa; 魔数, 用于判断错误
; loader读到0x1000开头两个字节为55aa

mov si, loading
call print

xchg bx, bx

detect_memory:
    ; 将 ebx 置为 0
    xor ebx, ebx

    ; es:di 结构体的缓存位置
    mov ax, 0
    mov es, ax
    mov edi, ards_buffer

    mov edx, 0x534d4150; 固定签名

.next:
    ; 子功能号
    mov eax, 0xe820
    ; ards 结构的大小 (字节)
    mov ecx, 20
    ; 调用 0x15 系统调用
    int 0x15

    ; 如果 CF 置位，表示出错
    jc error

    ; 将缓存指针指向下一个结构体
    add di, cx

    ; 将结构体数量加一
    inc dword [ards_count]

    cmp ebx, 0
    jnz .next

    mov si, detecting
    call print

    ; xchg bx, bx

    ; mov byte [0xb8000], 'P'

    jmp prepare_protected_mode

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
detecting:
    db "Detecting memory success...", 10, 13, 0

error:
    mov si, .msg
    call print
    hlt; 让 CPU 停止
    jmp $
    .msg db "Loading Error!!!", 10, 13, 0

ards_count:
    dw 0
ards_buffer:
