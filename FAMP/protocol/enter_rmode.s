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
    jmp dword 0x08:.pmode
.pmode:
    use32
    mov ax, 0x10
    mov ds, ax
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
%define unknown_protocol_revision       0x0002
%define bad_disk_image_end_sig          0x0003

;
; All error messages for the above error codes.
;
bad_disk_image_header_sig_err_msg:  db 'The disk image heading signature is invalid (Error Code: 0x0001)', 0x0
unknown_protocol_revision_err_msg:  db 'The disk image heading protocol revision is invalid (Error Code: 0x0002)', 0x0
bad_disk_image_end_sig_err_msg:     db 'The disk image heading ending signature is invalid (Error Code: 0x0003)', 0x0
unknown_err_msg: db 'An unknown error has occurred', 0x0

global enter_rmode_and_test
use32
enter_rmode_and_test:
    __x86_EnterRealMode

    push ebp
    mov ebp, esp
    
    mov ax, [ebp + 8]

    pop ebp

    cmp ax, AllGood
    je .ag

    cmp ax, bad_disk_image_header_sig
    je .bdihs   ; bad disk image header sig (bdihs)

    cmp ax, unknown_protocol_revision
    je .upr     ; unknown protocol revision (upr)

    cmp ax, bad_disk_image_end_sig
    je .bdihes  ; bad disk images header end sig (bdihes)

    mov si, unknown_err_msg
    call print

    jmp $
.bdihs:
    mov si, bad_disk_image_header_sig_err_msg
    call print

    jmp $
.upr:
    mov si, unknown_protocol_revision_err_msg
    call print

    jmp $
.bdihes:
    mov si, bad_disk_image_end_sig_err_msg
    call print

    jmp $
.ag:
    enter_protected_mode
    ret


global enter_pmode
use16
enter_pmode:
    enter_protected_mode
    
    ret

%include "protocol/bit16_IO/bit16_print.s"