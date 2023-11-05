#include "fs_worker.hpp"

//extern uint8 filesystem[];

/* TODO: Might delete and make the second stage do it all. */

void __START main()
{
    puint8 addr = (puint8) 0xB8000;

    struct FAMP_FS_HEADER *fs_header = (struct FAMP_FS_HEADER *) filesystem;
    struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *dimg_heading = (struct FAMP_PROTOCOL_DISK_IMAGE_HEADING *) FAMP_DISK_IMAGE_HDR_ADDR;
    dimg_heading->finalize_disk_image_heading();

    enter_rmode_and_test(dimg_heading->disk_image_is_good());

    enter_rmode();
    
    if(fs_header->FampFS_Sig[0] == 'F' && fs_header->FampFS_Sig[5] == 'S')
    {
        addr[2] = 'D';
        addr[4] = 'I';
        addr[6] = 'C';
        addr[8] = 'K';
    }

    while(true);
}