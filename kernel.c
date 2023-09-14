#define OS_RELATED
#define BIT32_PROGRAM
#include <common.hpp>

void __attribute__((section("__start"))) main(void)
{
    unsigned char *f = (unsigned char *) 0xB8000;
    f[0] = 'D';
    //__asm__("mov ah, 0x0E\nmov al, 'A'\nint 0x10");
    while(1);
}