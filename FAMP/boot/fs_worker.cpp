#include "fs_worker.hpp"

//extern uint8 filesystem[];

/* TODO: Might delete and make the second stage do it all. */

struct SMAP_entry {
	uint64  address;
	uint64  length;
	uint32 Type; // entry Type
	uint32 ACPI; // extended
}__attribute__((packed));

uint32 detectMemory(SMAP_entry* buffer, uint32 maxentries)
{
	uint32 contID = 0;
	int entries = 0;
    int signature;
    int bytes;
	do 
	{
		__asm__ __volatile__ ("int  0x15" 
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		
        if (signature != 0x534D4150) 
			return -1; // error
		
        if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
		{
            break;
			continue;
		}
		else {
			buffer++;
			entries++;
		}
	} 
	while (contID != 0 && entries < maxentries);

	return entries;
}

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