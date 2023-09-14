#ifndef FAMP_PROTOCOL_ADDRESSES_H
#define FAMP_PROTOCOL_ADDRESSES_H
#include "common.hpp"

#define MBR_ADDRESS                         0x7C00
//#define MBR_PART_TABLE_PROGRAM_ADDRESS      0x7E00 /* Dwells right after MBR. */
#define SECOND_STAGE_PROGRAM_ADDRESS        0x7E00 /* Second-stage bootloader dwells right after the MBR partitition table program. The last program read the second stage bootloader into memory. */
#define ABS_SECOND_STAGE_PROGRAM_ADDRESS    0x7E0D /* Absolute second stage address is the offset from the subheading as to where the actual program resides. */
#define FS_WORKER_PROGRAM_ADDRESS           0x1000
#define FILESYSTEM_ADDRESS                  0xF000

/* MBR partition table entries. */
#ifdef NEED_MBR_PART_TABLE_ENTRY_ADDRESSES
#define FAMP_SECTION_DATA                   (0x7C00 + 0x1B6)
#define FIRST_ENTRY                         (0x7C00 + 0x1BE)
#define SECOND_ENTRY                        (FIRST_ENTRY) + 0x10
#define THIRD_ENTRY                         (SECOND_ENTRY) + 0x10
#define FOURTH_ENTRY                        (THIRD_ENTRY) + 0x10
#endif

#endif