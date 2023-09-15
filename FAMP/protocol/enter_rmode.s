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

global enter_protected_mode
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

global enter_pmode
use16
enter_pmode:
    enter_protected_mode

    ret