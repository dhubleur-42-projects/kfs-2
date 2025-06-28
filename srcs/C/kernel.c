#include "display.h"
#include "interrupts.h"
#include "keyboard.h"
#include "gdt.h"

void test() {
	print_kernel_stack();
}

void kernel_main() {
	vga_clear();

	idt_init();

	enable_interrupts();

	test();

	for(;;) {
		asm volatile("hlt");
	}
}