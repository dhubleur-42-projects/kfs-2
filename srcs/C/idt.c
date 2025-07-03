#include "idt.h"

#include "io.h"
#include "vga.h"

extern uint32_t idt_table;

//TODO AVOID uint64_t
void set_gate(uint8_t i_gate, uint32_t offset, uint16_t seg_selector_index, uint64_t flags)
{
    uint64_t descriptor;
	uint32_t *tmp_address; //TEMP
	tmp_address = &idt_table;
	descriptor = GATE_OFFSET(offset) | SEG_SELECTOR_INDEX(seg_selector_index) | flags;
	tmp_address[i_gate*2+1] = (descriptor & 0xFFFFFFFF00000000) >> 32;
	tmp_address[i_gate*2] = descriptor & 0xFFFFFFFF;
}

__attribute__ ((interrupt))
void interrupt_keyboard_handler (struct interrupt_frame *frame)
{
	(void)frame;
	
	uint8_t a = io_inb(0x60);
	io_outb(0x20,0x20);
	terminal_writestring("OMG");
}

void pouet()
{
	asm volatile("cli"); //TEMP
	for (int i = 1; i < 255; i++)
	{
		set_gate(i, (uint32_t)&interrupt_keyboard_handler, 2, INTERRUPT_PL0);
	}
	asm volatile("sti"); //TEMP

	while (1)
	{
		asm volatile("hlt"); //TEMP
	}
}
