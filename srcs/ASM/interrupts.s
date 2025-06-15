global load_idt
global default_interrupt_handler_wrapper
global keyboard_handler_wrapper

extern default_interrupt_handler
extern keyboard_handler

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

default_interrupt_handler_wrapper:
    pusha
    call default_interrupt_handler
    popa
    iretd

keyboard_handler_wrapper:
    pusha
    call keyboard_handler
    popa
    iretd