#include "interrupts.h"

idt_entry_t idt[IDT_SIZE];
idt_ptr_t idtp;

void idt_set_gate(int n, uint32_t handler) {
    unsigned short cs;
    __asm__ ("mov %%cs, %0" : "=r" (cs));

    idt[n].base_low = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].selector = cs;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
}

void idt_init() {
    idtp.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idtp.base = (uint32_t)&idt;

	// Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_gate(i, 0);
    }

    load_idt(&idtp);

	pic_remap();

	idt_set_gate(33, (uint32_t)keyboard_handler_wrapper);
}