data segment
  n dw ?
  str_n db 'Enter n in decimal: $'
  ans db 10 dup(0)
  output db  'Number in Hexa : $'
  len dw 0
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
  mov cx,bx

  call PrintRegister
  lea dx, output
  mov ah, 09h
  int 21h
  call PrintAns

  ; int 3
  mov ah,1h 
  int 21h




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


; result is in cx
PrintRegister proc
    push ax
    push bx
    xor ax,ax
    lea si,ans

PrintLoop:
    mov bx,len
    inc bx
    mov len,bx
    mov ax,cx
    xor dx,dx
    mov bx,16
    div bx
    mov cx,ax
    mov bx,dx
    mov [si],bx
    ; int 3
    inc si
    cmp cx,0
    jnz PrintLoop

ExitPrintLoop:
   pop bx
   pop ax
   ret

PrintRegister endp


PrintAns proc
   mov cx,len
   dec si
  ;  int 3
PrintAnsLoop:
   mov dl,[si]
  ;  int 3
   cmp dl,10
   jb PrintHex
   ja PrintChar
   add dl,030h

PrintHex:
  add dl,30h
  mov ah,02h
  int 21h
  sub dl,30h
  xor ax,ax
  cmp ax,0
  jz ContinueLoop

PrintChar:
  add dl,37h
  mov ah,02h
  int 21h 
  sub dl,37h  
  xor ax,ax 
  cmp ax,0
  jz ContinueLoop

ContinueLoop:
  dec si
  dec cx
  cmp cx,0
  jnz PrintAnsLoop
  call EndProc

EndProc:
   ret

PrintAns endp

; Prints newline
PrintNextLine proc
    push ax
    push dx
    
    mov dl, 10
    mov ah, 02h
    int 21h
    mov dl, 13
    mov ah, 02h
    int 21h

    pop dx
    pop ax
    ret
PrintNextLine endp



code ends
end start