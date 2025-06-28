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

int vga_print_number(int idx, int num, vga_memory_t color) {
	if (idx < 0 || idx >= VGA_CAPACITY) return idx;

	if (num == 0) {
		vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char('0', color));
		return idx + 1;
	}

	if (num < 0) {
		vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char('-', color));
		num = -num;
		idx++;
		if (idx >= VGA_CAPACITY) return idx;
	}

	int digits[10];
	int count = 0;
	while (num > 0 && count < 10) {
		digits[count++] = num % 10;
		num /= 10;
	}

	for (int i = count - 1; i >= 0 && idx < VGA_CAPACITY; --i) {
		vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char('0' + digits[i], color));
		idx++;
	}

	return idx;
}

int vga_print_string(int idx, const char *str, vga_memory_t color) {
	if (idx < 0 || idx >= VGA_CAPACITY || str == NULL) return idx;

	for (int i = 0; str[i] != '\0' && idx < VGA_CAPACITY; ++i) {
		vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char(str[i], color));
		idx++;
	}

	return idx;
}

void vga_printf(int x, int y, vga_memory_t color, const char *fmt, ...) {
	int idx = x + y * VGA_WIDTH;
	if (idx < 0 || idx >= VGA_CAPACITY) return;

	uint32_t *va_list = (uint32_t *)(&fmt + 1);

	for (int i = 0; fmt[i] != '\0' && idx < VGA_CAPACITY; ++i) {
		if (fmt[i] == '%' && fmt[i + 1] != '\0') {
			switch (fmt[i + 1]) {
				case 'i':
				case 'd': {
					int value = *(int *)va_list;
					va_list++;
					idx = vga_print_number(idx, value, color);
					break;
				}
				case 'c': {
					char c = *(char *)va_list;
					va_list++;
					vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char(c, color));
					idx++;
					break;
				}
				case 's': {
					const char *str = *(const char **)va_list;
					va_list++;
					idx = vga_print_string(idx, str, color);
					break;
				}
				case 'x': {
					unsigned int hex_value = *(unsigned int *)va_list;
					va_list++;
					char hex_str[9];
					hex_str[8] = '\0';
					for (int j = 7; j >= 0; --j) {
						int digit = hex_value & 0xF;
						hex_str[j] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
						hex_value >>= 4;
					}
					idx = vga_print_string(idx, hex_str, color);
					break;
				}
				case 'X': {
					unsigned int hex_value = *(unsigned int *)va_list;
					va_list++;
					char hex_str[9];
					hex_str[8] = '\0';
					for (int j = 7; j >= 0; --j) {
						int digit = hex_value & 0xF;
						hex_str[j] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
						hex_value >>= 4;
					}
					idx = vga_print_string(idx, hex_str, color);
					break;
				}
				case 'p': {
					void *ptr_value = *(void **)va_list;
					va_list++;
					unsigned int hex_value = (unsigned int)ptr_value;
					char hex_str[9];
					hex_str[8] = '\0';
					for (int j = 7; j >= 0; --j) {
						int digit = hex_value & 0xF;
						hex_str[j] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
						hex_value >>= 4;
					}
					idx = vga_print_string(idx, hex_str, color);
					break;
				}
				default:
					vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char(fmt[i], color));
					idx++;
					vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char(fmt[i + 1], color));
					idx++;
					break;
			}
			i++;
		} else {
			vga_putchar(idx % VGA_WIDTH, idx / VGA_WIDTH, vga_color_char(fmt[i], color));
			idx++;
		}
	}
}

extern uint32_t stack_bottom;
extern uint32_t stack_top;

void print_kernel_stack() {
	vga_memory_t default_color = vga_fg_color(VGA_COLOR_LIGHT_GREY);
    vga_memory_t header_color = vga_fg_color(VGA_COLOR_LIGHT_RED);
    vga_memory_t stack_color = vga_fg_color(VGA_COLOR_LIGHT_GREEN);
    vga_memory_t esp_color = vga_fg_color(VGA_COLOR_LIGHT_MAGENTA);

    int y_pos = 0;

    vga_printf(0, y_pos++, header_color, "KERNEL STACK DUMP:");
    vga_printf(0, y_pos++, header_color, "Stack range: 0x%x - 0x%x",
              &stack_bottom, &stack_top);

    uint32_t current_esp;
    asm volatile ("mov %%esp, %0" : "=r"(current_esp));

    vga_printf(0, y_pos++, esp_color, "Current ESP: 0x%x", current_esp);

    uint32_t stack_used = (uint32_t)(&stack_top) - current_esp;
    vga_printf(0, y_pos++, default_color, "Stack used: %d bytes", stack_used);

    y_pos++;

    uint32_t* esp_ptr = (uint32_t*)current_esp;

    int items_to_show = 32;
    int items_shown = 0;

    vga_printf(0, y_pos++, header_color, "Stack contents (top to bottom):");

    while (esp_ptr < (uint32_t*)&stack_top && items_shown < items_to_show) {
        if ((uint32_t)esp_ptr >= (uint32_t)&stack_bottom) {
            uint32_t value = *esp_ptr;
            if (esp_ptr == (uint32_t*)current_esp) {
                vga_printf(0, y_pos++, esp_color, "> 0x%x: 0x%x",
                          (uint32_t)esp_ptr, value);
            } else {
                vga_printf(0, y_pos++, stack_color, "  0x%x: 0x%x",
                          (uint32_t)esp_ptr, value);
            }
            esp_ptr++;
            items_shown++;
        } else {
            break;
        }
    }

    if (items_shown < items_to_show && (uint32_t)esp_ptr < (uint32_t)&stack_bottom) {
        y_pos++;
        vga_printf(0, y_pos++, header_color, "... (bottom of stack reached)");
    }
}