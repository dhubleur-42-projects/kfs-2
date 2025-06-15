ENTRY(start)
SECTIONS {
    . = 1M;

    .multiboot : {
        *(.multiboot);
    }: text

    .text : {
        *(.text);
    }: text
    
    .rodata : {
        *(.rodata)
    }: text

    .data : {
        *(.data)
    }: data

    .bss : {
        *(.bss);
    }: data
}

PHDRS {
    text PT_LOAD FLAGS(5); /* R-X */
    data PT_LOAD FLAGS(6); /* RW- */
}