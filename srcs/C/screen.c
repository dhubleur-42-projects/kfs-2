#include "screen.h"

screen_t screens[SCREEN_COUNT];
int current_screen_index = 8;

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
		screen->buffer[i] = VGA_EMPTY_ENTRY;
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
	screen->buffer[start_x + screen_x_length + VGA_WIDTH * 2] = vga_color_char('1' + index, text_color2);

	vga_memory_t border_color = vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	for (int x = 1; x < VGA_WIDTH - 1; x++) {
		screen->buffer[x + VGA_WIDTH * 3] = vga_color_char('-', border_color);
		screen->buffer[x + VGA_WIDTH * 20] = vga_color_char('-', border_color);
	}
	for (int y = 3; y < VGA_HEIGHT - 4; y++) {
		screen->buffer[1 + VGA_WIDTH * y] = vga_color_char('|', border_color);
		screen->buffer[78 + VGA_WIDTH * y] = vga_color_char('|', border_color);
	}
	screen->buffer[1 + VGA_WIDTH * 3] = vga_color_char('#', border_color);
	screen->buffer[78 + VGA_WIDTH * 3] = vga_color_char('#', border_color);
	screen->buffer[1 + VGA_WIDTH * 20] = vga_color_char('#', border_color);
	screen->buffer[78 + VGA_WIDTH * 20] = vga_color_char('#', border_color);

	vga_memory_t text_color3 = vga_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);

	int instr1_length = 18;
	for (int i = 0; i < instr1_length; i++) {
		screen->buffer[2 + VGA_WIDTH * 22 + i] = vga_color_char("Type what you want"[i], text_color3);
	}
	int instr2_length = 29;
	for (int i = 0; i < instr2_length; i++) {
		screen->buffer[2 + VGA_WIDTH * 23 + i] = vga_color_char("Press F1-F9 to switch screens"[i], text_color3);
	}
	int instr3_length = 24;
	for (int i = 0; i < instr3_length; i++) {
		screen->buffer[2 + VGA_WIDTH * 24 + i] = vga_color_char("F9 is the command screen"[i], text_color3);
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

void add_to_current_screen(char c) {
	screen_t *current_screen = &screens[current_screen_index];

	vga_entry_t print_char = vga_char(c);
	current_screen->buffer[current_screen->cursor_x + VGA_WIDTH * current_screen->cursor_y] = print_char;
	vga_putchar(current_screen->cursor_x, current_screen->cursor_y, print_char);

	current_screen->cursor_x++;
	if (current_screen->cursor_x >= 78) {
		current_screen->cursor_x = 2;
		current_screen->cursor_y++;
		if (current_screen->cursor_y >= 20) {
			current_screen->cursor_y = 4;
		}
	}
}

void add_newline_to_current_screen() {
	screen_t *current_screen = &screens[current_screen_index];

	current_screen->cursor_x = 2;
	current_screen->cursor_y++;
	if (current_screen->cursor_y >= 20) {
		current_screen->cursor_y = 4;
	}
}

void back_cursor(screen_t *screen) {
	if (screen->cursor_x > 2) {
		screen->cursor_x--;
	} else if (screen->cursor_y > 4) {
		screen->cursor_y--;
		screen->cursor_x = 77;
	} else {
		screen->cursor_x = 77;
		screen->cursor_y = 19;
	}
}

void delete_last_char_from_current_screen() {
	screen_t *current_screen = &screens[current_screen_index];

	int init_x = current_screen->cursor_x;
	int init_y = current_screen->cursor_y;
	back_cursor(current_screen);

	for (;;) {
		vga_entry_t cur_char = current_screen->buffer[current_screen->cursor_x + VGA_WIDTH * current_screen->cursor_y];
		if (cur_char != VGA_EMPTY_ENTRY) {
			break;
		}
		if (current_screen->cursor_x == init_x && current_screen->cursor_y == init_y) {
			return;
		}
		back_cursor(current_screen);
	}

	current_screen->buffer[current_screen->cursor_x + VGA_WIDTH * current_screen->cursor_y] = VGA_EMPTY_ENTRY;
	vga_putchar(current_screen->cursor_x, current_screen->cursor_y, VGA_EMPTY_ENTRY);
}

int get_current_screen_index()
{
	return current_screen_index;
}

void get_current_screen_line(uint8_t y, char line_buf[80])
{
	for (int i = 0; i < 80; i++)
	{
		line_buf[i] = screens[current_screen_index].buffer[y * VGA_WIDTH + i];
	}
}

void get_current_pos(uint8_t *x, uint8_t *y)
{
	*x = screens[current_screen_index].cursor_x;
	*y = screens[current_screen_index].cursor_y;
}
