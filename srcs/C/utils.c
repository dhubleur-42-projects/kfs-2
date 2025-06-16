#include "utils.h"

char to_upper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - ('a' - 'A');
	}
	return c;
}