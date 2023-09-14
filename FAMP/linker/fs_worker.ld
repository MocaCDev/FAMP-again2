ENTRY(main)
SECTIONS {
    /* Program starts at 0x0700. */
    . = 0xA000;

    filesystem = 0xF000;

    .text : AT(0xA000)
    {
        *(__start*);
        *(.text*);
    }

    .data :
    {
        *(.bss*);
        *(.data*);
        *(.rodata*);
        *(COMMON*);
    }

}