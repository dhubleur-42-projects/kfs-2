#include "print_stack.h"

#include <stdint.h>

#include "vga.h"

extern uint32_t stack_top;

void print_stack()
{
	uint32_t *stack_ptr;

	__asm__("mov %0, esp\n" : "=r"(stack_ptr));
	terminal_writestring("--- Kernel stack ---\n");
	for (uint32_t *cur_ptr = stack_ptr; cur_ptr <= &stack_top; cur_ptr += 4)
	{
		terminal_writestring("0x");
		terminal_write_hex(*cur_ptr);
		terminal_putchar('\n');
	}
}
