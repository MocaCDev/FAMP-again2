#include "fs_worker.hpp"

//extern uint8 filesystem[];

/* TODO: Might delete and make the second stage do it all. */

void __START main()
{
    //if(filesystem[0] == 0x46)
    //{
        puint8 addr = (puint8) 0xB8000;
        addr[0] = 'D';
        addr[2] = 'I';
        addr[4] = 'C';
        addr[6] = '1';
    //}

    while(true);
}