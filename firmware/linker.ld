MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 10K
FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 32K
}

SECTIONS
{
    .text : {
        /* Start at 0 for the code in flash */
        . = 0;

        /* At the very beginning, we need the interrupt vectors.
        * We need to mark this KEEP to make sure it doesn't get garbage collected.
        * The syntax *(.foo) means take the .foo sections from all files. */
        KEEP(*(.interrupt_vectors))

        /* The whole interrupt table is 304 bytes long. Advance to that position in case
        * the supplied table was shorter. */
        . = 304;

        /* And here comes the rest of the code, ie. all symbols starting with .text */
        *(.text*)
    } > FLASH = 0xFF /* Put this in the flash memory region, default to 0xFF */

    _end_of_ram = ORIGIN(RAM) + LENGTH(RAM); /* Define a symbol for the end of ram */
}