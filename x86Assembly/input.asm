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
  call ReadNumber

  mov n,bx


  mov ax, 0
  mov bx, 1
  mov cx, 0

  push dx
  


InitLoop:
  mov di, n
  sub di, 2

Loop_i:
  mov cx, bx
  add cx, ax

  mov ax, bx 
  mov bx, cx 

  dec di
  cmp di, 0
  jg Loop_i

  int 3


; reads a number and stores it in bx , ends if read character is not in [0..9]
ReadNumber proc
   ; we will be using ax,bx,cx so store them in stack
   push ax
   push bx
   push dx

   ; reading number in bx register
   xor bx,bx

; as int 21 h gets only character by character we have to loop until we hit a non digit
ReadNumberLoop:
   xor ax,ax
   mov ah,1h    ; 1h is for input and 2h is for output both are related to al
   int 21h

   ; val now stored in al 
   ; Note : its stored in ascii so compare it with characters and later convert it into decimal by subtracting ascii value of '0' == 30h
   ; check if its in [0..9]

   
   cmp al,','
   jz ExitRead   
   cmp al,'9'
   ja ExitRead  ; above 9
   cmp al,'0'
   jb ExitRead  ; below 0


   

   
    
   ;convert ascii to hexa
   sub al,30h
   ; now we have al, but ax might be 1h or 2h so extend its sign 
   ; use cbw --> convert byte to word ,extends sign of al to ax
   cbw
   ; use cwd --> convert word to double , extends sign of ax to dx so now its 32 bit number
   cwd



   ; main op
   ; initially bx is 0 but later we need to add bx = (bx*10)+ax
   ; first push ax to stack as mult is done by ax
   push ax
   mov ax,bx
   ; mul 10 gives illegal immediate error
   ; so use cx for that
   mov cx,10
   mul cx

   mov bx,ax ; moving bx*10 to bx
   pop ax  ; pop old ax value and add it to bx
   add bx,ax
   
   jmp ReadNumberLoop ; run until we hit non digit character


ExitRead:
  ; pop in reverse order
  pop dx
  pop cx
  pop ax
  ret
ReadNumber endp ; end procedure


code ends
end start