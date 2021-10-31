data segment
  n dw ?
  r dw ?
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
  
  int 3

  mov ah, 4ch     
  int 21h

code ends
end start
