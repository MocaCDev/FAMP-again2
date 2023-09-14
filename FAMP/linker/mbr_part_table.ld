ENTRY(main)
SECTIONS {
    /* Program starts at 0x0500. */
    . = 0x8400;

    .text : AT(0x8400)
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