#include "io.h"

void send_byte_to_port(uint16_t port, uint8_t value) {
	asm volatile (
		"outb %%al, %%dx"
		:
		: "a"(value), "d"(port)
	);
}

uint8_t read_byte_from_port(uint16_t port) {
	uint8_t value;
	asm volatile (
		"inb %%dx, %%al"
		: "=a"(value)
		: "d"(port)
	);
	return value;
}

void send_short_to_port(uint16_t port, uint16_t value) {
	asm volatile (
		"outw %%ax, %%dx"
		:
		: "a"(value), "d"(port)
	);
}
