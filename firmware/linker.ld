MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 10K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 32K
}

SECTIONS
{
    .text : 
    {
        . = ALIGN(4);
        KEEP(*(.interrupt_vectors))
        . = 304;                        /* Skip to end of interrupt vectors */
        *(.text*)                       /* Program code */
        *(.rodata*)                     /* Read only data */
        . = ALIGN(4);
		_etext = .;
    } > FLASH = 0xFF /* Put this in the flash memory region, default to 0xFF */

    _sidata = .;

	.data ORIGIN(RAM) :  AT ( ADDR (.text) + SIZEOF (.text) )
	{
		_sdata = .;
		*(.data*)                       /* Read-write initialized data */
		. = ALIGN(4);
		_edata = .;
	}

	.bss ADDR (.data) + SIZEOF (.data):
	{
		_sbss = .;
		. = ALIGN(4);
		*(.bss*)                        /* Read-write zero initialized data */
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	}

    _eram = ORIGIN(RAM) + LENGTH(RAM);  /* Symbol for the end of ram */
}