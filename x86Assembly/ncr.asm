DATA SEGMENT
    n dw ?
    r dw ?
    enter_n db 'Enter n : $'
    enter_r db 'Enter r : $'
    ans db 10 dup(0)
    output db  'Result : $'
    len dw 0
DATA ENDS

code segment
    assume cs:code,ds:data	
start:	      
    mov ax,data
    mov ds,ax
    
    
    lea dx, enter_n
    mov ah, 09h
    int 21h
    call ReadNumber
    mov n,bx
    lea dx, enter_r
    mov ah, 09h
    int 21h
    call ReadNumber
    mov r,bx
    mov ax,n
    mov bx,r
    mov cx,0
    ; int 3

call encr
call PrintRegister
; int 3
lea dx, output
mov ah, 09h
int 21h
call PrintAns
call PrintNextLine
xor ax,ax
mov ah, 4h     
int 21h


encr proc
    cmp ax,bx
    je ncr1
    cmp bx,0
    je ncr1
    cmp bx,1
    je ncrn
    dec ax
    cmp bx,ax
    je ncrn1
    push ax
    push bx
    call encr
    pop bx
    pop ax
    dec bx
    push ax
    push bx
    call encr
    pop bx
    pop ax
    ret
ncr1: 	      inc cx
     ret
ncrn1:	     inc ax
ncrn:	     add cx,ax
     ret
encr endp


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

; result is in cx
PrintRegister proc
    push ax
    push bx
    xor ax,ax
    xor bx,bx
    lea si,ans

PrintLoop:
    mov bx,len
    inc bx
    mov len,bx
    mov ax,cx
    xor dx,dx
    mov bx,10
    div bx
    mov cx,ax
    mov bx,dx
    mov [si],bx
    inc si
    cmp cx,0
    jnz PrintLoop

ExitPrintLoop:
   pop bx
   pop ax
   ret

PrintRegister endp


PrintAns proc
   mov cx,0
   mov bx,len
   dec si
PrintAnsLoop:
   mov ah,02h
   mov dl,[si]
   add dl,30h
   int 21h  
   dec si
   inc cx
   cmp cx,len
   jnz PrintAnsLoop 
     
   ret 
PrintAns endp


code ends
end start