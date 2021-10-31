data segment
  mat1 db 1h, 1h, 1h, 2h, 2h, 2h, 3h , 3h , 3h

  mat2 db 1h, 1h, 1h, 1h, 1h, 1h

  mat3 db 09h dup(?) 

  aa db 3h
  bb db 3h
  cc db 2h

  a dw 3h
  b dw 3h
  c dw 2h

data ends

code segment
assume cs:code, ds:data
start:
  mov ax, data
  mov ds, ax

  mov ch, aa 

  mov bx, offset mat3 
  mov si, offset mat1

NextRow:
  mov di, offset mat2

  mov cl, cc 

NextCol:
  mov dl, bb
  
  mov bp, 0h

NextElement:
  mov ax, 0h
  mov al, [si]
  mul byte ptr[di]
  add bp, ax
  inc si 
  add di, c 

  dec dl   
  jnz NextElement

  mov [bx], bp

  sub si, b

  mov ax, b
  mul c
  dec ax
  sub di, ax

  inc bx

  dec cl 

  cmp cl, 0
  jnz NextCol

  add si, b

  dec ch

  cmp ch, 0
  jnz NextRow

  int 3

code ends
end start