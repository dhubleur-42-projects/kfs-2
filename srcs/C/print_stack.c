#include "vga.h"

#include <stdint.h>

static void print_hex_nbr(uint32_t nbr);

extern uint32_t stack_top;

void print_stack()
{
	uint32_t *stack_ptr;

	__asm__("mov %0, esp\n" : "=r"(stack_ptr));
	terminal_writestring("--- Kernel stack ---\n");
	for (uint32_t *cur_ptr = stack_ptr; cur_ptr <= &stack_top; cur_ptr += 4)
	{
		terminal_writestring("0x");
		print_hex_nbr(*cur_ptr);
		terminal_putchar('\n');
	}
}

static void print_hex_nbr(uint32_t nbr)
{
	const char *HEX_DIGITS = "0123456789ABCDEF";

	if (nbr == 0)
		return;
	print_hex_nbr(nbr / 16);
	terminal_putchar (HEX_DIGITS[nbr % 16]);
}
