#include "display.h"
#include "interrupts.h"

void kernel_main() {
	vga_clear();
    vga_putchar(0, 0, vga_color_char('4', vga_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE)));
	vga_putchar(1, 0, vga_color_char('2', vga_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_LIGHT_MAGENTA)));

	vga_printf(0, 2, vga_fg_color(VGA_COLOR_WHITE), "Hello %i %i %c %s", 42, -42, '!', "World");

	idt_init();
	enable_interrupts();

	for(;;) {
		asm volatile("hlt");
	}
}