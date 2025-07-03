#include "idt.h"

#include "io.h"
#include "vga.h"

extern uint32_t idt_table;

//TODO AVOID uint64_t
void set_gate(uint8_t i_gate, uint32_t offset, uint16_t seg_selector_index, uint8_t seg_selector_ti, uint8_t seg_selector_rpl, uint8_t flags)
{
    uint32_t descriptor_lo;
    uint32_t descriptor_hi;
	uint32_t *idt_table_ptr;

	idt_table_ptr = &idt_table;

	descriptor_lo = offset & 0xFFFF;
	descriptor_lo |= (SEG_SELECTOR(seg_selector_index, seg_selector_ti, seg_selector_rpl) << 16); //TODO Inside struct for seg?

	descriptor_hi = ((flags & 0xFF) << 8);
	descriptor_hi |= ((offset & 0xFFFF0000) << (16-16));

	idt_table_ptr[i_gate*2+1] = descriptor_hi;
	idt_table_ptr[i_gate*2] = descriptor_lo;
}

__attribute__ ((interrupt))
void interrupt_keyboard_handler (struct interrupt_frame *frame)
{
	(void)frame;
	
	uint8_t a = io_inb(0x60);
	io_outb(0x20,0x20);
	terminal_writestring("OMG");
}
