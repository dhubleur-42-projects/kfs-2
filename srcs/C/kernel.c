#include "vga.h"
#include "idt.h"

void print_stack();

void kernel_main(void)
{
	terminal_initialize();
	terminal_writestring("42\n");
	print_stack();
	PIC_remap();
	set_gate(33, (uint32_t)&interrupt_keyboard_handler, 1, INTERRUPT_PL0);
	asm volatile("sti");
	while (1)
	{
		asm volatile("hlt"); //TEMP
	}
}
