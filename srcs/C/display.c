#include "display.h"

inline vga_memory_t vga_color(vga_color_e fg, vga_color_e bg) {
	return (bg << 4) | (fg & 0x0F);
}

inline vga_memory_t vga_fg_color(vga_color_e fg) {
	return vga_color(fg, VGA_COLOR_BLACK);
}

inline vga_entry_t vga_color_char(char c, vga_memory_t color) {
	return (vga_entry_t)c | ((vga_entry_t)color << 8);
}

inline vga_entry_t vga_char(char c) {
	return vga_color_char(c, vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
}

void vga_putchar(int x, int y, vga_entry_t c) {
	if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
	VGA_MEMORY[y * VGA_WIDTH + x] = c;
}

void vga_clear(void) {
	for (int y = 0; y < VGA_HEIGHT; ++y) {
		for (int x = 0; x < VGA_WIDTH; ++x) {
			vga_putchar(x, y, vga_color_char(' ', vga_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK)));
		}
	}
}