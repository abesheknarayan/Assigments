data segment
  arr dw  5h, 4h, 3h, 2h, 1h
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
  jc Continue 

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
  mov ah,2
  mov dx,[si]
  int 21h
  mov ax, [si]
  add si, 2
  dec ch
  jnz printArr

  int 3

code ends
end start