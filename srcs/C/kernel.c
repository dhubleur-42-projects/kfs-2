#include "display.h"
#include "interrupts.h"
#include "screen.h"
#include "keyboard.h"

void kernel_main() {
	vga_clear();
	reset_screens();

	idt_init();
	enable_interrupts();

	for(;;) {
		asm volatile("hlt");
		char c = get_last_pressed_char();
		if (c != 0) {
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
	}
}