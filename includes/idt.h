#ifndef IDT_H
# define IDT_H

# include <stdint.h>

#define GATE_OFFSET(x)		(((((uint64_t)x) & 0xFFFF0000) << (48 - 16)) | (((x) & 0x0000FFFF)))
#define GATE_PRES(x)      (((uint64_t)x) << 47)			// Present
#define GATE_PRIV(x)     ((((uint64_t)x) & 3) << 45)		// Set privilege level (0 - 3)

#define GATE_TYPE_TASK			((uint64_t)0x5 << 40)
#define GATE_TYPE_16B_INTERRUPT	((uint64_t)0x6 << 40)
#define GATE_TYPE_16B_TRAP		((uint64_t)0x7 << 40)
#define GATE_TYPE_32B_INTERRUPT	((uint64_t)0xE << 40)
#define GATE_TYPE_32B_TRAP		((uint64_t)0xF << 40)

#define SEG_SELECTOR_INDEX(x)	((x) << (3+16))
#define SEG_SELECTOR_TI(x)		((x) << (2+16))		// 0 to use GDT, 1 to use LDT
#define SEG_SELECTOR_RPL(x)		(((x) & 0x3) << 16)		// Privilege level (0 - 3)

#define INTERRUPT_PL0 GATE_PRES(1) | GATE_PRIV(0) | GATE_TYPE_32B_INTERRUPT | SEG_SELECTOR_TI(0) | SEG_SELECTOR_RPL(0)

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
void set_gate(uint8_t i_gate, uint32_t offset, uint16_t seg_selector_index, uint64_t flags);

#endif
