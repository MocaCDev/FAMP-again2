#include "fs_worker.hpp"

//extern uint8 filesystem[];

/* TODO: Might delete and make the second stage do it all. */

void __START main()
{
    enter_rmode();

    if(filesystem[0] == 0x46)
    {
        puint8 addr = (puint8) 0xB8000;
        addr[2] = 'D';
        addr[4] = 'I';
        addr[6] = 'C';
        addr[8] = 'K';
    }

    while(true);
}