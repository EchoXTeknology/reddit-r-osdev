[org 0x0]
[bits 16]

%define SECTORS 16
%define IMAGE_SIZE ((SECTORS + 1) * 512) ; This will increase in size later on...
%define STACK_SIZE 256


; Global Entry point for bootsector
_glEntry:
    cli     ; Disable interrupts
    
    ; Setup Stack Pointers
    mov ax, (((SECTORS + 1) * 32) + 4000 + STACK_SIZE)
    mov ss, ax
    mov sp, (STACK_SIZE * 16)
    
    sti     ; Enable interrupts
    
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov ax, 0x0
    int 0x13
    
    jc .disk_reset_error
    
    push es
    
    mov ax, 0x07E0
    mov es, ax
    mov bx, 0x0
    
    mov ah, 0x02
    mov al, SECTORS
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    int 0x13
    
    jc .disk_read_error
    
    pop es
    
    mov si, BOOT_MSG
    call .sprint
    
    ; Ref _glExtentedBootsector
    jmp 0x07E0:0x0000
  
  
  
.disk_reset_error:
    mov si, DISK_RESET_ERROR_MSG
    jmp .fatal
    
.disk_read_error:
    mov si, DISK_READ_ERROR_MSG
    
.fatal:
    call .sprint
    
    ; Wait for user input...
    mov ax, 0x0
    int 0x16
    
    ; Perform BIOS "Warm-Boot/Reboot"
    mov ax, 0x0
    int 0x19
    
.sprint:
    mov ah, 0x0E
    
    .repeat:
        lodsb
        or al, al
        jz .end
        int 0x10
        jmp .repeat
        
    .end:
        ret
        
        
DISK_RESET_ERROR_MSG db 'ERROR::DISK::RESET_FAILURE!', 0x0A, 0x0D, 0x0
DISK_READ_ERROR_MSG db 'ERROR::DISK::READ_FAILURE!', 0x0A, 0x0D, 0x0
BOOT_MSG db 'Booting...', 0x0A, 0x0D, 0x0

times 510-($-$$) db 0x0
dw 0xAA55

; 0x07E0 Positioned
_glExtentedBootsector:
    ; Wait for user input...
    mov ax, 0x0
    int 0x16
    
    ; Perform BIOS shutdown
    mov ax, 0x5301
    xor bx, bx
    int 0x15
    
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15
    
times IMAGE_SIZE-($-$$) db 0x0
