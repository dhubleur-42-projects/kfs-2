#include "display.h"
#include "interrupts.h"
#include "keyboard.h"

void kernel_main() {
	vga_clear();

	idt_init();
	enable_interrupts();

	for(;;) {
		asm volatile("hlt");
		char c = get_last_pressed_char();
		if (c != 0) {
		}
	}
}