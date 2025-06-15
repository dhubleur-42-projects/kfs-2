#ifndef INTERUPTS_H
# define INTERUPTS_H

# include "types.h"
# include "io.h"

#define IDT_SIZE 256

typedef struct {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

extern void load_idt(idt_ptr_t*);
extern void default_interrupt_handler_wrapper();
extern void keyboard_handler_wrapper();

void idt_init();
void pic_remap();
void disable_interrupts();
void enable_interrupts();
void send_eoi(uint8_t irq);

void default_interrupt_handler();
void keyboard_handler();

#endif