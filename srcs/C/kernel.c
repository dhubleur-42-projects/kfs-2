#include "display.h"
#include "interrupts.h"

void kernel_main() {
	idt_init();
	enable_interrupts();

	vga_clear();
    vga_putchar(0, 0, vga_color_char('4', vga_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE)));
	vga_putchar(1, 0, vga_color_char('2', vga_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_LIGHT_MAGENTA)));

	for(;;) {
		asm volatile("hlt");
	}
}