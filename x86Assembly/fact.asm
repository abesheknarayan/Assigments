
data segment
  a dw 0009h
  b dd ?
data ends

code segment
assume cs:code, ds:data

start:
  mov ax, data
  mov ds, ax

  mov ax, a 
  mov bx, a

  cmp bx, 01
  jnz factorial
factorial:
  dec bx
  mul bx
  cmp bx, 01
  jnz factorial

  ;mov b, ax
  lea si,b
  mov [si],ax
  mov [si+2],dx
  int 3

code ends
end start