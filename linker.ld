ENTRY(start)

SECTIONS {
    . = 1M;

    .multiboot : {
        *(.multiboot)
    }

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        . = ALIGN(16);
        stack_bottom = .;
        . = . + 16384;
        stack_top = .;
    }
}