working_bit32_bit16_GDT:      ; NULL descriptor
            .null_desc          dq 0x0

            ; 32-bit code segment
            .code32_limit       dw 0xFFFF                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .code32_base        dw 0                        ; base (bits 0-15) = 0x0
            .code32_base2       db 0                        ; base (bits 16-23)
            .code32_acces       db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
            .code32_gran        db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .code32_base_high   db 0                        ; base high

            ; 32-bit data segment
            .data32_limit       dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .data32_base        dw 0                        ; base (bits 0-15) = 0x0
            .data32_base2       db 0                        ; base (bits 16-23)
            .data32_access      db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
            .data32_gran        db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .data32_base_high   db 0                       ; base high

            ; 16-bit code segment
            .cod16_limit        dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF
            .code16_base        dw 0                        ; base (bits 0-15) = 0x0
            .code16_base2       db 0                        ; base (bits 16-23)
            .code16_access      db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
            .code16_gran        db 00001111b                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
            .code16_base_high   db 0                        ; base high

            ; 16-bit data segment
            .data16_limit       dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF
            .data16_base        dw 0                        ; base (bits 0-15) = 0x0
            .data16_base2       db 0                        ; base (bits 16-23)
            .data16_access      db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
            .data16_gran        db 00001111b                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
            .data16_base_high   db 0                        ; base high

working_bit32_bit16_GDTDesc:
    dw working_bit32_bit16_GDTDesc - working_bit32_bit16_GDT - 1
    dd working_bit32_bit16_GDT

global __load_gdt
use16
__load_gdt:
    in al, 0x92
    or al, 0x02
    out 0x92, al

    cli
    lgdt [working_bit32_bit16_GDTDesc]

    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp word 0x8:_init_pm

global _init_pm
use32
_init_pm:

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp word 0x8:0x840D