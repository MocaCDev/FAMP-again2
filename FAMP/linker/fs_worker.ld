ENTRY(main)
SECTIONS {
    /* Program starts at 0x0700. */
    . = 0xB000;

    filesystem = 0x8200;

    .text : AT(0xB000)
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