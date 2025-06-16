#ifndef DISPLAY_H
# define DISPLAY_H

# include "types.h"

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_e;

typedef uint8_t vga_memory_t;
typedef uint16_t vga_entry_t;

# define VGA_WIDTH 80
# define VGA_HEIGHT 25
# define VGA_CAPACITY (VGA_WIDTH * VGA_HEIGHT)
# define VGA_MEMORY_SIZE (VGA_CAPACITY * 2)
# define VGA_ADDRESS 0xB8000
# define VGA_MEMORY ((volatile vga_entry_t *)VGA_ADDRESS)

vga_memory_t vga_color(vga_color_e fg, vga_color_e bg);
vga_memory_t vga_fg_color(vga_color_e fg);

vga_entry_t vga_color_char(char c, vga_memory_t color);
vga_entry_t vga_char(char c);

void vga_putchar(int x, int y, vga_entry_t c);
void vga_printf(int x, int y, vga_memory_t color, const char *fmt, ...);
void vga_clear(void);

#endif