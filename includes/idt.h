#ifndef IDT_H
# define IDT_H

# include <stdint.h>

#define SEG_SELECTOR(index,ti,rpl)		(((index) << 3) | ((ti & 1) << 2) | (rpl & 3))



#define GATE_OFFSET(x)		(((((uint64_t)x) & 0xFFFF0000) << (48 - 16)) | (((x) & 0x0000FFFF)))
#define GATE_PRES(x)      ((x) << 7)			// Present
#define GATE_PRIV(x)     (((x) & 3) << 5)		// Set privilege level (0 - 3)

#define GATE_TYPE_TASK			0x5
#define GATE_TYPE_16B_INTERRUPT	0x6
#define GATE_TYPE_16B_TRAP		0x7
#define GATE_TYPE_32B_INTERRUPT	0xE
#define GATE_TYPE_32B_TRAP		0xF

#define INTERRUPT_PL0 GATE_PRES(1) | GATE_PRIV(0) | GATE_TYPE_32B_INTERRUPT

struct interrupt_frame
{
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};

__attribute__ ((interrupt))
void interrupt_keyboard_handler (struct interrupt_frame *frame);
void set_gate(uint8_t i_gate, uint32_t offset, uint16_t seg_selector_index, uint8_t seg_selector_ti, uint8_t seg_selector_rpl, uint8_t flags);

#endif
