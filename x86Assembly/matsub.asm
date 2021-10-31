data segment
  matrix1 dw 000Ah, 4h, 6h, 5h, 1h, 3h

  matrix2 dw 4h, 1h, 1h, 1h, 1h, 1h

  a dw 3h
  b dw 2h

data ends

code segment
assume cs:code, ds:data
start:
    mov ax, data
    mov ds, ax

    mov ax, a
    mul b

    mov cx, ax
    lea si, matrix1
    lea di, matrix2

forLoop:
    mov ax, [si]
    mov bx, [di]
    sub ax, bx
    
    mov [si], ax

    add si, 2h
    add di, 2h
    sub cx, 0001h
    
    cmp cx, 0
    jnz forLoop

    int 3

code ends
end start 