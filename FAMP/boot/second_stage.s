[org 0x7E00]
use16

;
; Per the current revision of FAMPs spec, if `IsAsmProgram` is true (1)
; there needs to be a assembly program header that follows directly after the
; programs subheader
;
FAMP_PROTOCOL_SUBHEADING:
    .SubHeadingSig          db 'FSHD'
    .padding                dw 0x0000
    .ProgramSizeInBytes     dd 1024
    .ProgramSizeInSectors   dw 2
    .IsAsmProgram           db 1

;
; `AsmProgramName` is allowed up to 15 characters.
; If `InRealMode` is true (1), then `UsesRealMode` must be true (1).
; 
; If `InRealMode` is true (1) and it uses long mode functionality,
; `UsesLongMode` must be true (1).
; 
; If `InRealMode` is false (0), then `UsesLongMode` must be true(1).
;
; If the program jumps to a program that's in long mode, of will be returning
; to a program that is in long mode, `ReturnsToLongMode` must be true (1).
;
FAMP_PROTOCOL_ASM_PROGRAM_HEADING:
    .AsmProgramHeaderSig    db 'ASM`, 0x0
    .AsmProgramName         db 'SecondStage    '
    .InRealMode             db 1
    .UsesRealMode           db 1
    .UsesLongMode           db 0
    .ReturnsToLongMode      db 1
    .padding                db 0x0 ; padding to make the assembly program header 24 bytes

mov ah, 0x00
mov al, 0x02
int 0x10

%include "protocol/bootloader/mbr_part_table.s"

;
; Reading in the FileSystem (FS).
;
xor ax, ax
mov es, ax
mov bx, 0xF000

mov al, SECOND_ENTRY_SECTOR_AMOUNT
mov cl, SECOND_ENTRY_START_SECTOR
call read_disk

;
; Reading in the program that relocates the FileSystem (FS) to its higher-half address,
; with this the program also extracts the kernels binary from the FileSystem (FS) and relocates
; it.
; The kernel will reside from the beginning of virtual memory, and the FileSystem (FS) will dwell
; after the last byte of the kernel.
;
mov ax, 0x0A00
mov es, ax
xor bx, bx

mov al, THIRD_ENTRY_SECTOR_AMOUNT
mov cl, THIRD_ENTRY_START_SECTOR
call read_disk

; Load the GDT and go to the program that relocates the FileSystem (FS) and kernel
call __load_gdt

jmp $

%include "boot/gdt.s"
%include "protocol/bootloader/read_disk.s"

failed:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10

    cli
    hlt
    jmp $

times (1024 - 8) - ($ - $$) db 0x0

FAMP_PROTOCOL_MEMORY_STAMP:
    .MemID      dw 0xF210
    .MemIDSig   db 'FEND'
    .padding    dw 0x0000