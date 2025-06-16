#include "display.h"
#include "interrupts.h"
#include "screen.h"

void kernel_main() {
	vga_clear();
	reset_screens();

	idt_init();
	enable_interrupts();

	for(;;) {
		asm volatile("hlt");
	}
}