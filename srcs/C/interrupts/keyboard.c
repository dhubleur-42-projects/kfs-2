#include "interrupts.h"

void keyboard_handler() {
    uint8_t scancode = read_byte_from_port(0x60);
    send_byte_to_port(0x20, 0x20);

    // Exemple simple : afficher le scancode
    volatile char* vmem = (char*)0xB8000;
    vmem[6] = '0' + (scancode % 10);
    vmem[7] = 0x07;
}