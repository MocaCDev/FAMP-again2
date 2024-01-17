#include "fs_worker.hpp"

extern struct SMAP_entry smap[];

void __START main()
{
    puint8 addr = (puint8) 0xB8000;

    struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *dimg_heading = (struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *) FAMP_DISK_IMAGE_HDR_ADDR;
    dimg_heading->finalize_disk_image_heading();
    dimg_heading->check_disk_image_heading(error_control_addr);

    struct FAMP_FS_HEADER *fs_header = (struct FAMP_FS_HEADER *) (fs_addr);
    fs_header->finalize_fs_header();

    fs_addr += sizeof(*fs_header);
    struct FAMP_FS_PARTITION_METADATA *kernel_partition_metadata = (struct FAMP_FS_PARTITION_METADATA *) (fs_addr);
    kernel_partition_metadata->finalize_partition_metadata();

    fs_addr += sizeof(*kernel_partition_metadata);
    
    uint32 i = 0;
    while(i < kernel_partition_metadata->PartitionByteSize)
    {
        kernel_virtual_address[i] = *fs_addr;
        fs_addr++;
        i++;
    }

    /* Move the FS header to the next available address directly following the last byte of the kernel. */
    struct FAMP_FS_HEADER *new_fs_header = (struct FAMP_FS_HEADER *) new_filesystem_address;
    *new_fs_header = *fs_header;

    fs_header = nullptr;

    enter_rmode_and_stay();

    __asm__("jmp 0x0:0x7EE9");

    while(true);
}