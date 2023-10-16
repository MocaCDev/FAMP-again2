#ifndef FAMP_PROTOCOL_BOOTLOADER_STRUCTURES_H
#define FAMP_PROTOCOL_BOOTLOADER_STRUCTURES_H
#include "common.hpp"

/* TODO: Do we need these? We also define them in `protocol/bootloader/mbr_part_table.s`. */
//#define ENTRY_TYPE_SECOND_STAGE       0x0E
//#define ENTRY_TYPE_FS                 0x0C
//#define ENTRY_TYPE_FS_WORKER          0x0D

#ifdef OS_RELATED
#include "addresses.hpp"

#define FAMP_DISK_IMAGE_HDR_OFFSET      0x05    /* There is a `jmp` instruction at the beginning of the MBR. */
#define FAMP_DISK_IMAGE_HDR_ADDR        (MBR_ADDRESS + FAMP_DISK_IMAGE_HDR_OFFSET)
#endif

namespace BootloaderStructs
{
    struct partition_entry
    {
        uint8       bootable_entry;
        uint16      starting_sector;
        uint8       entry_type;
        bool        auto_read_program;
        uint16      last_sector;
        uint32      relative_sector;
        uint8       sector_amnt;
        uint32      padding;

        partition_entry() = default;

        ~partition_entry() = default;
    } __attribute__((packed));
}

#endif