#include "second_stage.hpp"

void __START main()
{
    //__asm__(".code16gcc");

    //__cpu_mode_pm_to_rm();
    //enter_protected_mode();

    //__asm__ volatile("jmp 0x8:0x1000");

    //puint8 addr = (puint8) 0xB8000;
    //addr[0] = 'g';
    //next();

    read_in_memory(0x0A00, 0x04, 0x02);

    __load_gdt();
    __asm__ volatile("jmp 0x8:0x1000");

    while(true);
}