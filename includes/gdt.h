#ifndef GDT_H
# define GDT_H

# include "types.h"
# include "utils.h"

typedef struct {
	uint16_t limit_low;      // Lower 16 bits of the limit
	uint16_t base_low;       // Lower 16 bits of the base address
	uint8_t  base_middle;    // Next 8 bits of the base address
	uint8_t  access;         // Access flags, determine what ring this segment can be used in
	uint8_t  granularity;    // Granularity and upper 4 bits of the limit
	uint8_t  base_high;      // Last 8 bits of the base address
} __attribute__((packed)) gdt_entry_t;

typedef struct {
	uint16_t limit;          // Size of the GDT in bytes
	uint32_t base;           // Base address of the GDT
} __attribute__((packed)) gdt_ptr_t;

# define GDT_ENTRIES 7

# define GDT_KERNEL_CODE 1
# define GDT_KERNEL_DATA 2
# define GDT_KERNEL_STACK 3
# define GDT_USER_CODE 4
# define GDT_USER_DATA 5
# define GDT_USER_STACK 6

extern void load_gdt(gdt_ptr_t *gdt_ptr);

#endif