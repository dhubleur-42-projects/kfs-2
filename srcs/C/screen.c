#include "screen.h"

screen_t screens[SCREEN_COUNT];
int current_screen_index = 0;

void print_screen_to_vga(int index) {
	if (index < 0 || index >= SCREEN_COUNT) {
		return;
	}

	screen_t *screen = &screens[index];
	for (int i = 0; i < VGA_CAPACITY; i++) {
		vga_putchar(i % VGA_WIDTH, i / VGA_WIDTH, screen->buffer[i]);
	}
}

void clear_screen(int index) {
	if (index < 0 || index >= SCREEN_COUNT) {
		return;
	}

	screen_t *screen = &screens[index];
	screen->cursor_x = 2;
	screen->cursor_y = 4;

	for (int i = 0; i < VGA_CAPACITY; i++) {
		screen->buffer[i] = vga_color_char(' ', vga_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK));
	}

	vga_memory_t text_color = vga_color(VGA_COLOR_BLUE, VGA_COLOR_LIGHT_MAGENTA);
	int welcome_length = 19;
	int start_x = (VGA_WIDTH - welcome_length) / 2;
	for (int i = 0; i < welcome_length; i++) {
		screen->buffer[start_x + i] = vga_color_char("Welcome to HideurOS"[i], text_color);
	}

	vga_memory_t text_color2 = vga_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_DARK_GREY);
	int screen_x_length = 7;
	start_x = (VGA_WIDTH - screen_x_length) / 2;
	for (int i = 0; i < screen_x_length; i++) {
		screen->buffer[start_x + i + VGA_WIDTH * 2] = vga_color_char("Screen X"[i], text_color2);
	}
	screen->buffer[start_x + screen_x_length + VGA_WIDTH * 2] = vga_color_char('0' + index, text_color2);

	vga_memory_t white_color = vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	for (int x = 1; x < VGA_WIDTH - 1; x++) {
		screen->buffer[x + VGA_WIDTH * 3] = vga_color_char('-', white_color);
		screen->buffer[x + VGA_WIDTH * 20] = vga_color_char('-', white_color);
	}
	for (int y = 3; y < VGA_HEIGHT - 4; y++) {
		screen->buffer[1 + VGA_WIDTH * y] = vga_color_char('|', white_color);
		screen->buffer[78 + VGA_WIDTH * y] = vga_color_char('|', white_color);
	}
	screen->buffer[1 + VGA_WIDTH * 3] = vga_color_char('#', white_color);
	screen->buffer[78 + VGA_WIDTH * 3] = vga_color_char('#', white_color);
	screen->buffer[1 + VGA_WIDTH * 20] = vga_color_char('#', white_color);
	screen->buffer[78 + VGA_WIDTH * 20] = vga_color_char('#', white_color);

	vga_memory_t text_color3 = vga_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);

	int instr1_length = 18;
	for (int i = 0; i < instr1_length; i++) {
		screen->buffer[2 + VGA_WIDTH * 23 + i] = vga_color_char("Type what you want"[i], text_color3);
	}
	int instr2_length = 29;
	for (int i = 0; i < instr2_length; i++) {
		screen->buffer[2 + VGA_WIDTH * 24 + i] = vga_color_char("Press F1-F9 to switch screens"[i], text_color3);
	}

	if (index == current_screen_index) {
		print_screen_to_vga(index);
	}
}

void reset_screens() {
	for (int i = 0; i < SCREEN_COUNT; i++) {
		clear_screen(i);
	}
}

void switch_screen(int index) {
	if (index < 0 || index >= SCREEN_COUNT) {
		return;
	}

	current_screen_index = index;
	print_screen_to_vga(current_screen_index);
}