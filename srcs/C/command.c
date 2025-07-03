#include "command.h"

#include "screen.h"
#include "types.h"
#include "interrupts.h"

int cmd_equals(char *cmd1, char *cmd2);
void reboot();
void halt();
static void print_hex_nbr(uint32_t nbr);
void print_stack();
void clear_all_screen();

void handle_command()
{
	uint8_t x, y, last_y;
	char line_buf[81];

	get_current_pos(&x, &y);
	if (y == 4)
		last_y = 19;
	else
		last_y = y - 1;

	get_current_screen_line(last_y, line_buf);
	line_buf[80] = ' ';
	if (cmd_equals(line_buf + 2, "reboot"))
		reboot();
	if (cmd_equals(line_buf + 2, "stack"))
		print_stack();
	if (cmd_equals(line_buf + 2, "halt"))
		halt();
	if (cmd_equals(line_buf + 2, "clear"))
		clear_screen(CMD_SCREEN_ID);
	if (cmd_equals(line_buf + 2, "clear all"))
		clear_all_screen();
}

int cmd_equals(char *cmd1, char *cmd2)
{
	int i;
	for (i = 0; cmd2[i] != 0; i++)
	{
		if (cmd1[i] != cmd2[i])
			return 0;
	}
	return 1;
}

void reboot()
{
	idt_ptr_t idtp;

	idtp.limit = 0;
	idtp.base = 0;
	load_idt(&idtp);
	asm volatile ("sti");
	asm volatile ("int3");
}

extern uint32_t stack_top;

void print_stack()
{
	uint32_t *stack_ptr;

	__asm__("mov %%esp, %0\n" : "=r"(stack_ptr));

	for (int i = 0; i < 22; i++) {
		add_to_current_screen("---- Kernel stack ----"[i]);
	}
	add_newline_to_current_screen();

	for (uint32_t *cur_ptr = stack_ptr; cur_ptr <= &stack_top; cur_ptr += 4)
	{
		add_to_current_screen('0');
		add_to_current_screen('x');
		if (*cur_ptr != 0)
			print_hex_nbr(*cur_ptr);
		else
			add_to_current_screen('0');
		add_newline_to_current_screen();
	}
}

void halt()
{
	send_short_to_port(0x604, 0x2000);
}

void clear_all_screen()
{
	for (int i = 0; i < SCREEN_COUNT; i++)
		clear_screen(i);
}

static void print_hex_nbr(uint32_t nbr)
{
	const char *HEX_DIGITS = "0123456789ABCDEF";

	if (nbr == 0)
		return;
	print_hex_nbr(nbr / 16);
	add_to_current_screen(HEX_DIGITS[nbr % 16]);
}
