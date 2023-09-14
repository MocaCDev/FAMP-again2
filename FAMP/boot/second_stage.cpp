#include "second_stage.hpp"

void __START main()
{
    if(filesystem[0] == 0x46)
    {
        //__asm__("mov ah, 0x0E\nmov al, 'T'\nint 0x10");
    
    //__load_gdt();
    //_init_pm();

        puint8 addr = (puint8) 0xB8000;
        addr[0] = 'D';
        addr[2] = 'I';
        addr[4] = 'C';
        addr[6] = 'K';
    }

    //__asm__ volatile("jmp 0x8:0x1000");

    //next();

    while(true);
}