#ifndef FAMP_PROTOCOL_MBR_PART_TABLE_H
#define FAMP_PROTOCOL_MBR_PART_TABLE_H
#define OS_RELATED
#include <common.hpp>
#define NEED_MBR_PART_TABLE_ENTRY_ADDRESSES
#include <addresses.hpp>
#include <bootloader/bootloader_structures.hpp>

using namespace BootloaderStructs;

extern "C" void __load_gdt();
extern "C" void _init_pm();

/*#include <FFF/FFF_structures.hpp>
#include <FFS/FAMP_fs.hpp>
#include <bootloader/bootloader_structures.hpp>

#define SECOND_STAGE_ADDRESS        0x8400

using namespace FFF_Structures;
using namespace BootloaderStructs;
using namespace FAMP_FS;

static struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *heading = (struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *) 0x0B00;
static struct MBR_bin_outline *entry_code = (struct MBR_bin_outline *) 0x0C84;
static struct partition_entry *pentry = (struct partition_entry *) 0x1000;
*/
#endif