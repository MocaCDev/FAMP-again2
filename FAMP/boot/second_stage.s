[org 0x7E00]
use16

start:

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
    .AsmProgramHeaderSig    db 'ASM', 0x0
    .AsmProgramName         db 'SecondStage    '
    .InRealMode             db 1
    .UsesRealMode           db 1
    .UsesLongMode           db 0
    .ReturnsToLongMode      db 1
    .padding                db 0x0 ; padding to make the assembly program header 24 bytes

mov ah, 0x00
mov al, 0x02
int 0x10

%define need_mbr_part_table_access
%include "protocol/protocol_util.s"

;
; Make sure the `entry_type`s are as they should be for each of the three
; MBR partition table entries.
;
mov ah, FIRST_ENTRY_TYPE
cmp ah, ENTRY_TYPE_SECOND_STAGE
jne failed

mov ah, SECOND_ENTRY_TYPE
cmp ah, ENTRY_TYPE_FS
jne failed

mov ah, THIRD_ENTRY_TYPE
cmp ah, ENTRY_TYPE_FS_WORKER
jne failed

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

; Get memory map
xor ax, ax
mov es, ax
mov di, ax

memmap_entries	equ 0x8500
get_mem_map:
    push bp
	mov di, 0x8504
	xor ebx, ebx
	xor bp, bp
	
	mov edx, 'PAMS'
	mov eax, 0xE820
	mov [es:di + 20], dword 1
	mov ecx, 24
	int 0x15
	jc .error
	
	cmp eax, 'PAMS'
	jne .error
	test ebx, ebx
	jz .error
	jmp .start
.next_entry:
	mov edx, 'PAMS'
	mov ecx, 24
	mov eax, 0xE820
	int 0x15
.start:
	jcxz .skip
	mov ecx, [es:di + 8]
	or ecx, [es:di + 12]
	jz .skip
.good:
	inc bp
	add di, 24
.skip:
	test ebx, ebx
	jz .done
	jmp .next_entry
.error:
	mov ah, 0x0e
	mov al, 'F'
	int 0x10

	cli
	hlt
.done:
	mov [memmap_entries], bp
    pop bp
    clc

; Load the GDT and go to the program that relocates the FileSystem (FS) and kernel
call __load_gdt

times 233 - ($ - $$) db 0x0

call enter_pmode
use32

;
; Since we do go to another program which returns back to 16-bit,
; we set up the stack directly before jumping to the kernel to avoid
; any corruption/complication.
;

mov ebp, 0xFFFFFFFF ; I have no idea if this is correct lol
mov eax, 0x1FFFFFFF
mov esp, eax

jmp word 0x8:0x20000000

jmp $

%include "boot/gdt.s"
%include "protocol/bootloader/read_disk.s"
%include "protocol/enter_rmode.s"

failed:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10

    cli
    hlt
    jmp $

times (1536 - 8) - ($ - $$) db 0x0

FAMP_PROTOCOL_MEMORY_STAMP:
    .MemID      dw 0xF210
    .MemIDSig   db 'FEND'
    .padding    dw 0x0000