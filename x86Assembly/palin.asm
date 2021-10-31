data segment
    str_inp db 'Enter string: $'
    str_success db 'OK - Input is palindrome$'
    str_fail db 'FAIL - Input is not palindrome$'
    new db '$'
    s db 20 dup(0)
data ends
code segment              
assume cs:code,ds:data
start:
    mov ax,data
    mov ds,ax

    lea dx, str_inp
    mov ah, 09h
    int 21h

    call ReadString
    call CheckPalidrome

    mov ah, 1h     
    int 21h

ReadString proc
    push ax
    mov bx, 00
ReadStringTakeMore:
    mov ah, 01h
    int 21h
    cmp al, 0dh
    je ReadStringDone
    mov [s+bx], al
    inc bx
    loop ReadStringTakeMore
ReadStringDone:
    pop ax
    ret
ReadString endp

CheckPalidrome proc
    push di
    
    mov di, 0
    dec bx
CheckPalidromeChar:
    mov al, [s+bx]
    cmp al, [s+di]
    jne CheckPalidromeFail
    inc di ; ++i
    dec bx ; --j
    jnz CheckPalidromeChar
    
    lea dx, str_success
    mov ah, 09h
    int 21h
    jmp CheckPalidromeEnd
CheckPalidromeFail:
    lea dx, str_fail
    mov ah,09h
    int 21h
CheckPalidromeEnd:
    pop di
    ret
CheckPalidrome endp

code ends
end start