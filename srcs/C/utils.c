#include "utils.h"

char to_upper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - ('a' - 'A');
	}
	return c;
}

void memcpy(void *dest, const void *src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char *)dest)[i] = ((const char *)src)[i];
	}
}