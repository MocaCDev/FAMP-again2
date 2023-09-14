;
; All of the critical locations of critical information in each entry.
; The "critical information" is the starting sector of the entry, alongside the
; sector amount.
;
%define FIRST_ENTRY_START_SECTOR		byte [0x7DBF]	; mbr_partition_entry.sector_amnt
%define FIRST_ENTRY_SECTOR_AMOUNT		byte [0x7DC9]	; mbr_partition_entry.starting_sector_and_cylinder

%define SECOND_ENTRY_START_SECTOR       byte [0x7DCF]
%define SECOND_ENTRY_SECTOR_AMOUNT      byte [0x7DD9]

%define THIRD_ENTRY_START_SECTOR        byte [0x7DDF]
%define THIRD_ENTRY_SECTOR_AMOUNT       byte [0x7DE9]

; The fourth entry doesn't matter. We know there is nothing there so.

;
; The MBR partition table entry structure
;
struc mbr_partition_table_entry
	; Is this MBR partition table entry bootable?(is it describing the rest of the bootloader?)
	; 0x80 = bootable, 0x0 = not bootable
	.is_bootable:									resb 0x01
	; From what sector does the program begin(lower 8-bits)
	; With this, the sector where the program begins has to be the sector where the last program ended off
	; So, for a program after the MBR, `first_sector` would be 2
	; From what cylinder does the sector reside on?(upper 8-bits)
	.starting_sector_and_cylinder:					resw 0x01
	; 0x05/0x0F for extended partition table, 0x01 denoting the entry is unused
	; Any other value has yet to be determined
	.entry_type:									resb 0x01
	; If this is `1`, the MBR partition table C program will automatically read in
	; the needed amount of memory. If `0`, the MBR partition table C program will not load
	; anything into memory.
	; This is going to be more helpful later on. With this, we can primarily mark certain
	; partitions in the extended partition table as "critical", thus, the protocol MUST
	; read the programs residing in each partition into memory AND run them
	.auto_read_program:								resb 0x01
	; From what sector does the program end?(lower 8-bits)
	; From what cylinder does the last sector reside on?(upper 8-bits)
	.last_sector_and_cylinder:						resw 0x01
	; Just set this to zero lol
	.relative_sector:								resd 0x01
	; How many sectors does the program obtain?
	; This amount * 512 will be added to `bprogram_address` to calculate the ending address
	; of the program
	.sector_amnt:									resb 0x01
	; Pad 1 byte
	.pad:											resd 0x01
endstruc

;
; Create the entries
;
%macro CreateEntry	7
mbr_partition_entry%+%1:	istruc mbr_partition_table_entry
	at mbr_partition_table_entry.is_bootable,					db %2
	at mbr_partition_table_entry.starting_sector_and_cylinder,	dw %3
	at mbr_partition_table_entry.entry_type,					db %4
	at mbr_partition_table_entry.auto_read_program,				db %5
	at mbr_partition_table_entry.last_sector_and_cylinder,		dw %6
	at mbr_partition_table_entry.relative_sector,				dd 0x0
	at mbr_partition_table_entry.sector_amnt,					db %7
	at mbr_partition_table_entry.pad,							dd 0x0
iend
%endmacro