data segment
  arr dw 0005h, 1003h, 4004h, 3002h, 0501h
  sz db 05h
data ends

code segment
assume cs:code, ds:data
start:
  mov ax, data
  mov ds, ax
  
  mov ch, sz
  dec ch
InitInnerLoop:

  mov cl, sz
  dec cl
  lea si, arr

InnerLoop:

  mov ax, [si]
  mov bx, [si + 2]
  cmp ax, bx
  jnc Continue 

  mov dx, [si + 2] 
  xchg [si], dx
  mov [si + 2], dx

Continue:
  add si, 2
  dec cl
  jnz InnerLoop
  dec ch
  jnz InitInnerLoop
  mov ch, sz
  lea si, arr
  
printArr:
  mov ax, [si]
  add si, 2
  dec ch
  jnz printArr

  int 3

code ends
end start