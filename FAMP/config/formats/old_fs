[org 0xF000]
use16

FAMP_FS_HEADER:
    .FampFS_Sig db      'FAMPFS'
    .FampFS_Rev dw      0x0001
    .FampFS_NOP dw      0x01
    .FampFS_NOF dd      0x0
    .padding            times 0xF6 dw 0x0
    .FampFS_BeginSig dd 0x47425346

FAMP_FS_PARTITION_METADA:
    .PartitionName db       'KERNEL'
    .padding                times (0x100 - 0x2) db 0x0
    .PartitionAcc db        0xD0 ; Protocol Only
    .PartitionAct db        0xF2 ; Nothing
    .PartitionAddress dd    0x80000000
    .PartitionAddressT db   0x1 ; Virtual
    .PartitionT db          0x0 ; Code
    .PartitionSS dd         0x1 ; SS - Sector Size
    .PartitionBS dd         0x200 ; BS - Byte Size (512 bytes, 0x200)
    .PartitionAC dw         0x0 ; AC - Action Count
    .PartitionAID db        0x0 ; 0x0 = neither protocol, kernel or user
    .PartitionDOFF dd       0x0 ; DOFF - Data Offset, kernel begins immediately
    .NeedsRelocated db      0x0 ; False
    .RequestedBy db         0x0 ; 0x0 = neither protocol, kernel or user
    .RelocationAddress dd   0x0
    .WillReturn db          0x0 ; False
    .OriginalAddress dd     0x0
    .RelocateAcc db         0x0 ; No relocation action
    .padding2               times 0xD7 db 0x0 ; 0xD7 = 0x100 - 41 = ((512 - 0x100) - 41)
    .PartitionBeginSig db   'PBEG'

incbin "../bin/kernel.bin"