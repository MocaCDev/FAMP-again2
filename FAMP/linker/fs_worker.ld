ENTRY(main)
SECTIONS {
    /* Program starts at 0x0700. */
    . = 0x1000;

    filesystem = 0xF000;

    .text : AT(0x1000)
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