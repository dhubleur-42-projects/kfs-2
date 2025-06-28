#include "display.h"
#include "interrupts.h"
#include "keyboard.h"
#include "gdt.h"

void kernel_main() {
	vga_clear();

	idt_init();

	enable_interrupts();

	vga_printf(0, 0, vga_fg_color(VGA_COLOR_WHITE), "Hello world !");

	for(;;) {
		asm volatile("hlt");
		char c = get_last_pressed_char();
		if (c != 0) {
		}
	}
}