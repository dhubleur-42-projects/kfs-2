#include "vga.h"

void print_stack();

void kernel_main(void)
{
	terminal_initialize();
	terminal_writestring("42\n");
	print_stack();
}
