assume cs:code,ds:data

data segment
n1 dw 0ffffh
n2 dw 4567h
n3 dw ?
data ends
 
code segment

start:
mov ax,data
mov ds,ax

mov ax,n1
mov bx,n2
sub ax,bx
mov n3,ax
lea si,n3
int 3

code ends
end start
