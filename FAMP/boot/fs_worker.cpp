#include "fs_worker.hpp"

//extern uint8 filesystem[];

/* TODO: Might delete and make the second stage do it all. */

void __START main()
{
    enter_rmode();

    struct FAMP_FS_HEADER *fs_header = (struct FAMP_FS_HEADER *) filesystem;

    if(fs_header->FampFS_Sig[0] == 'F' && fs_header->FampFS_Sig[5] == 'S')
    {
        puint8 addr = (puint8) 0xB8000;
        addr[2] = 'D';
        addr[4] = 'I';
        addr[6] = 'C';
        addr[8] = 'K';
    }

    while(true);
}