#ifndef FAMP_PROTOCOL_BOOTLOADER_STRUCTURES_H
#define FAMP_PROTOCOL_BOOTLOADER_STRUCTURES_H
#include "common.hpp"

#define ENTRY_TYPE_SECOND_STAGE       0x0E
#define ENTRY_TYPE_FS                 0x0C
#define ENTRY_TYPE_FS_WORKER          0x0D

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