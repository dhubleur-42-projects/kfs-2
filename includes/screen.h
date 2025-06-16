#ifndef SCREEN_H
# define SCREEN_H

# include "types.h"
# include "display.h"

# define SCREEN_COUNT 10

typedef struct {
	uint8_t cursor_x;
	uint8_t cursor_y;
	vga_entry_t buffer[VGA_CAPACITY];
} screen_t;

void print_screen_to_vga(int index);
void clear_screen(int index);
void reset_screens();
void switch_screen(int index);

#endif