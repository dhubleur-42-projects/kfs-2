global load_idt
global keyboard_handler_wrapper

extern keyboard_handler

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

keyboard_handler_wrapper:
    pusha
    call keyboard_handler
    popa
    iretd