#include "display.h"
#include "interrupts.h"
#include "screen.h"
#include "keyboard.h"
#include "command.h"

#ifdef BONUS
static void handle_typed_char(char c);
#endif

void kernel_main() {
#ifndef BONUS

	print_stack();
	asm volatile("hlt");

#else

	idt_init();
	enable_interrupts();

	vga_clear();
	reset_screens();

	for(;;) {
		asm volatile("hlt");
		char c = get_last_pressed_char();
		if (c >= 11 && c <= 19) {
			switch_screen(c - 11);
		} else if (c != 0) {
			handle_typed_char(c);
			if (c == '\n' && get_current_screen_index() == CMD_SCREEN_ID)
			{
				handle_command();
			}
		}
	}
	
#endif
}

#ifdef BONUS
static void handle_typed_char(char c)
{
	switch (c) {
		case '\n':
			add_newline_to_current_screen();
			break;
		case '\b':
			delete_last_char_from_current_screen();
			break;
		default:
			add_to_current_screen(c);
			break;
	}
}
#endif
