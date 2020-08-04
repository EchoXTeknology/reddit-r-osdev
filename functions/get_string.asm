; CODE IS NOT STANDALONE! ATTEMPTING BOOT WILL
; RESULT IN A BOOTING FAILURE!
[bits 16]



; Function Header...
get_string:
    xor cl, cl
    .loop:
        ; Collect User Input
        mov ah, 0x0
        int 0x16
        
        ; Was input Backspace?
        cmp al, 0x08
        je .backspace
        
        ; Was input Return/Enter?
        cmp al, 0x0D
        je .done
        
        ; Has user input reached 64 characters:
        ; 64-BIT_OFFSET=63 characters...
        cmp cl, 0x3F
        je .loop
        
        mov ah, 0x0E
        int 0x10
        
        stosb
        inc cl
        jmp .loop
        
    .backspace:
        cmp cl, 0x0
        je .loop
        
        dec di
        mov byte [di], 0x0
        dec cl
        
        mov ah, 0x0E
        mov al, 0x08
        int 0x10
        
        mov ah, ' '
        int 0x10
        
        mov al, 0x08
        int 0x10
        
        jmp .loop
        
    .done:
        mov al, 0x0
        stosb
        
        mov ah, 0x0E
        mov al, 0x0D
        int 0x10
        
        mov al, 0x0A
        int 0x10
        
        ret
