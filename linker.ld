ENTRY(start)
SECTIONS {
    .text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
	.data : { *(.data) }
	.bss : { *(.bss) }
}