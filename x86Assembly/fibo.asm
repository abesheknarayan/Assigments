data segment
  n dw ?
  str_n db 'Enter n: $'
data ends

code segment
assume cs:code, ds:data
start:
  mov ax, data
  mov ds, ax
  mov ah, 09h
  lea dx, str_n
  int 21h
  mov  ah , 01h
  int 21h
  sub al,30h
  cbw
  mov n,ax
  

  mov ax, 0
  mov bx, 1
  mov cx, 0
  
  push dx○


InitLoop:
  mov dx, n
  sub dx, 2

Loop_i:
  mov cx, bx
  add cx, ax

  mov ax, bx 
  mov bx, cx 

  dec dx
  cmp dx, 0
  jg Loop_i

  int 3

  mov ah,04h
  int 21h

code ends
end start