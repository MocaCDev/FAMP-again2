#include "mbr_part_table.hpp"

#define MBR_partition_table_entries_offset     0x1BE
#define second_MBR_partition_table_entry       (0x1BE + 0) + 16

void __START main()
{
    //*heading = *(struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *) FAMP_DISK_IMAGE_HDR_ADDR;
    //heading->HeaderSig = revert_value<uint32> (heading->HeaderSig);
    //MBR_ptr_addr += sizeof(*heading);

    /* MBR "entry" code outline. */
    struct partition_entry *pentry;
    //*entry_code = *(struct MBR_bin_outline *) MBR_ptr_addr;
    
    /* MBR partition table entries. */
    //*pentry = *(struct partition_entry *) (0x7C00 + MBR_partition_table_entries_offset);//MBR_ptr_addr;
    //read_in_memory(SECOND_STAGE_ADDRESS/16, pentry->starting_sector, pentry->sector_amnt);
    
    /* FS worker program. */
    //read_in_memory(0x0B00, pentry->starting_sector + pentry->sector_amnt, 1);

    /* Get the FS partition entry, just for possible future reference. */
    //*pentry = *(struct partition_entry *) (0x7C00 + second_MBR_partition_table_entry);

    //__asm__("jmp 0x0:0x7E0D");

    pentry = (struct partition_entry *) (FIRST_ENTRY);
    read_in_memory(SECOND_STAGE_PROGRAM_ADDRESS / 16, pentry->starting_sector, pentry->sector_amnt);

    //pentry = (struct partition_entry *) (THIRD_ENTRY);
    //read_in_memory(FS_WORKER_PROGRAM_ADDRESS / 16, pentry->starting_sector, pentry->sector_amnt);
    
    __load_gdt();
    //_init_pm();

    while(true);
}