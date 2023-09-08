#define OS_RELATED
#define BIT32_PROGRAM
#include <common.hpp>

void __attribute__((section("__start"))) main(void)
{
    unsigned char *f = (unsigned char *) 0xB8000;
    f[0] = 'a';
    while(1);
}