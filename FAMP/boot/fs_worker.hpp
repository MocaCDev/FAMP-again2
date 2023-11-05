#ifndef FAMP_PROTOCOL_FS_WORKER_H
#define FAMP_PROTOCOL_FS_WORKER_H
#define OS_RELATED
#define BIT32_PROGRAM
#include <common.hpp>
#include <FFS/FAMP_fs.hpp>
/* This program is, technically, still part of FAMPs "bootloader". */
#include <bootloader/rmode_functions.hpp>
#include <bootloader/bootloader_structures.hpp>
#include <FFF/FFF_structures.hpp>

using namespace FFF_Structures;
using namespace BootloaderStructs;
using namespace FAMP_FS;

extern uint8 filesystem[];
extern uint8 error_control_addr[];
extern uint8 kernel_virtual_address[];
uint8 *fs_addr = filesystem;

#endif