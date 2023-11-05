%macro __x86_EnterRealMode 0
    [bits 32]
    jmp word 18h:.__pmode16         ; 1 - jump to 16-bit protected mode segment

.__pmode16:
    [bits 16]
    ; 2 - disable protected mode bit in cr0
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    ; 3 - jump to real mode
    jmp word 00h:.__rmode

.__rmode:
    ; 4 - setup segments
    mov ax, 0x0
    mov ds, ax
    mov ss, ax

    ; 5 - enable interrupts
    sti

%endmacro

%macro enter_protected_mode 0

    cli

    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp word 0x08:.pmode
.pmode:
    use32
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

%endmacro

global rmode_print
use16
rmode_print:
    mov ah, 0x0E
    mov al, 'A'
    int 0x10

    ret

;
; TODO: This function will have to take an argument so it knows what real-mode function to call.
;       `enter_rmode` will be an assembly stub used by C/C++ programs that are running in 32-bit mode.
;       The stub will allow the program to enter real mode, perform a task then quickly get back to 32-bit mode.
;
global enter_rmode
use32
enter_rmode:
    __x86_EnterRealMode
    call rmode_print
    enter_protected_mode

    ret

;
; For now, this is for errors. We will change this later (TODO)
;
global enter_rmode_and_stay
use32
enter_rmode_and_stay:
    __x86_EnterRealMode

    ret

;
; All error codes for `enter_rmode_and_test`
; 
; `AllGood` is not an error code. If this is the value passed to `enter_rmode_and_test`,
; the stub will just return back to the program.
;
%define AllGood                         0x0000
%define bad_disk_image_header_sig       0x0001
%define invalid_mjr_revision            0x0002
%define invalid_mnr_revision            0x0003
%define bad_disk_image_end_sig          0x0004

;
; All error messages for the above error codes.
;
bad_disk_image_header_sig_err_msg:  db 'The disk image heading signature is invalid', 0xD, 0xA, '    (Error Code: 0x0001)', 0xD, 0xA, 0x0
invalid_mjr_revision_err_msg:       db 'The disk image heading protocol major revision is invalid', 0xD, 0xA, '    (Error Code: 0x0002)', 0xD, 0xA, 0x0
invalid_mnr_revision_err_msg:       db 'The disk image heading protocol minor revision is invalid', 0xD, 0xA, '    (Error Code: 0x0003)', 0xD, 0xA, 0x0
bad_disk_image_end_sig_err_msg:     db 'The disk image heading ending signature is invalid', 0xD, 0xA, '    (Error Code: 0x0004)', 0xD, 0xA, 0x0
unknown_err_msg: db 'An unknown error has occurred', 0xD, 0xA, 0x0

;
; The address `0x1000` to `0x1001` (1 byte) will tell the program if an error occurred.
;
error_control_addr equ 0x1000

global enter_rmode_and_raise_err
use32
enter_rmode_and_raise_err:
    __x86_EnterRealMode

    push ebp
    mov ebp, esp
    
    mov ax, [ebp + 8]

    pop ebp

    cmp ax, AllGood
    je .ag

    cmp ax, bad_disk_image_header_sig
    je .bdihs   ; bad disk image header sig (bdihs)

    cmp ax, invalid_mjr_revision
    je .imajr     ; invalid major revision (imajr)

    cmp ax, invalid_mnr_revision
    je .iminr    ; invalid minor revision (iminr)

    cmp ax, bad_disk_image_end_sig
    je .bdihes  ; bad disk images header end sig (bdihes)

    mov si, unknown_err_msg
    call print

    jmp .r
.bdihs:
    mov si, bad_disk_image_header_sig_err_msg
    call print

    mov byte [error_control_addr], 0x1 ; true

    jmp .r
.imajr:
    mov si, invalid_mjr_revision_err_msg
    call print

    mov byte [error_control_addr], 0x1 ; true

    jmp .r
.iminr:
    mov si, invalid_mnr_revision_err_msg
    call print

    mov byte [error_control_addr], 0x1 ; true

    jmp .r
.bdihes:
    mov si, bad_disk_image_end_sig_err_msg
    call print

    mov byte [error_control_addr], 0x1 ; true

    jmp .r
.ag:
    jmp .r;mov byte [error_control_addr], 0x0 ; false
.r:
    enter_protected_mode
    ret

global enter_rmode_and_hlt
use32
enter_rmode_and_hlt:
    __x86_EnterRealMode

    cli
    hlt
    jmp $

global enter_pmode
use16
enter_pmode:
    enter_protected_mode
    
    use32
    ret

global enter_rmode_n
use32
enter_rmode_n:
    __x86_EnterRealMode

    push ebp
    mov ebp, esp
    
    mov si, [ebp + 8]

    pop ebp

    pusha
    call print

    mov ah, 0x0E
    mov al, ' '
    int 0x10
    popa

    enter_protected_mode
    ret

%include "protocol/bit16_IO/bit16_print.s"