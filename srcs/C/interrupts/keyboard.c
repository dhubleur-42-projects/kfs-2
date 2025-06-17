#include "interrupts.h"
#include "keyboard.h"
#include "display.h"

bool isLeftShiftPressed = false;
char lastPressedChar = 0;

char keyboard_get_char(keyboard_key_t key) {
    switch (key) {
        case KEYBOARD_KEY_A: return (isLeftShiftPressed ? 'A' : 'a');
        case KEYBOARD_KEY_B: return (isLeftShiftPressed ? 'B' : 'b');
        case KEYBOARD_KEY_C: return (isLeftShiftPressed ? 'C' : 'c');
        case KEYBOARD_KEY_D: return (isLeftShiftPressed ? 'D' : 'd');
        case KEYBOARD_KEY_E: return (isLeftShiftPressed ? 'E' : 'e');
        case KEYBOARD_KEY_F: return (isLeftShiftPressed ? 'F' : 'f');
        case KEYBOARD_KEY_G: return (isLeftShiftPressed ? 'G' : 'g');
        case KEYBOARD_KEY_H: return (isLeftShiftPressed ? 'H' : 'h');
        case KEYBOARD_KEY_I: return (isLeftShiftPressed ? 'I' : 'i');
        case KEYBOARD_KEY_J: return (isLeftShiftPressed ? 'J' : 'j');
        case KEYBOARD_KEY_K: return (isLeftShiftPressed ? 'K' : 'k');
        case KEYBOARD_KEY_L: return (isLeftShiftPressed ? 'L' : 'l');
        case KEYBOARD_KEY_M: return (isLeftShiftPressed ? 'M' : 'm');
        case KEYBOARD_KEY_N: return (isLeftShiftPressed ? 'N' : 'n');
        case KEYBOARD_KEY_O: return (isLeftShiftPressed ? 'O' : 'o');
        case KEYBOARD_KEY_P: return (isLeftShiftPressed ? 'P' : 'p');
        case KEYBOARD_KEY_Q: return (isLeftShiftPressed ? 'Q' : 'q');
        case KEYBOARD_KEY_R: return (isLeftShiftPressed ? 'R' : 'r');
        case KEYBOARD_KEY_S: return (isLeftShiftPressed ? 'S' : 's');
        case KEYBOARD_KEY_T: return (isLeftShiftPressed ? 'T' : 't');
        case KEYBOARD_KEY_U: return (isLeftShiftPressed ? 'U' : 'u');
        case KEYBOARD_KEY_V: return (isLeftShiftPressed ? 'V' : 'v');
        case KEYBOARD_KEY_W: return (isLeftShiftPressed ? 'W' : 'w');
        case KEYBOARD_KEY_X: return (isLeftShiftPressed ? 'X' : 'x');
        case KEYBOARD_KEY_Y: return (isLeftShiftPressed ? 'Y' : 'y');
        case KEYBOARD_KEY_Z: return (isLeftShiftPressed ? 'Z' : 'z');
        case KEYBOARD_KEY_0: return (isLeftShiftPressed ? ')' : '0');
        case KEYBOARD_KEY_1: return (isLeftShiftPressed ? '!' : '1');
        case KEYBOARD_KEY_2: return (isLeftShiftPressed ? '@' : '2');
        case KEYBOARD_KEY_3: return (isLeftShiftPressed ? '#' : '3');
        case KEYBOARD_KEY_4: return (isLeftShiftPressed ? '$' : '4');
        case KEYBOARD_KEY_5: return (isLeftShiftPressed ? '%' : '5');
        case KEYBOARD_KEY_6: return (isLeftShiftPressed ? '^' : '6');
        case KEYBOARD_KEY_7: return (isLeftShiftPressed ? '&' : '7');
        case KEYBOARD_KEY_8: return (isLeftShiftPressed ? '*' : '8');
        case KEYBOARD_KEY_9: return (isLeftShiftPressed ? '(' : '9');
        case KEYBOARD_KEY_SPACE: return ' ';
        case KEYBOARD_KEY_ENTER: return '\n';
        case KEYBOARD_KEY_BACKSPACE: return '\b';
        default: return '?';
    }
}

void keyboard_handler() {
    uint8_t scancode = read_byte_from_port(0x60);
    send_byte_to_port(0x20, 0x20);

    keyboard_key_t key = KEYBOARD_KEY(scancode);
    if (scancode == KEYBOARD_KEY_SHIFT_LEFT) {
        isLeftShiftPressed = (KEYBOARD_EVENT(scancode) == KEY_PRESSED);
    } else if (KEYBOARD_EVENT(scancode) == KEY_PRESSED) {
        lastPressedChar = keyboard_get_char(key);
    }

    send_eoi(1);
}

char get_last_pressed_char() {
    char c = lastPressedChar;
    lastPressedChar = 0; // Clear the last pressed character after retrieving it
    return c;
}