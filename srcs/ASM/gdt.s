global load_gdt

extern stack_top 
load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]

    mov ecx, [esp]
    mov esp, stack_top
    push ecx
    
    jmp 0x08:.reload_segments

.reload_segments:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ret