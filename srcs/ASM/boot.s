bits 32

%define MAGIC 0x1BADB002       
%define MULTIBOOT_FLAGS 0  
%define CHECKSUM - (MAGIC + MULTIBOOT_FLAGS)

section .multiboot
	dd MAGIC			; Multiboot magic number
	dd MULTIBOOT_FLAGS	; Multiboot flags
	dd CHECKSUM			; Checksum for the multiboot header

section .gdt
gdt_table:
dq 0x0000000000000000 ; NULL Entry
dq 0x00CF9A000000FFFF ; Kernel code segment
dq 0x00CF92000000FFFF ; Kernel data segment
dq 0x00CF92000000FFFF ; Kernel stack segment
dq 0x00CFFA000000FFFF ; User code segment
dq 0x00CFF2000000FFFF ; User data segment
dq 0x00CFF2000000FFFF ; User stack segment
gdt_table_size: equ $ - gdt_table

section .text
	global start

	extern kernel_main

	start:
		mov esp, stack_top
        call setup_gdt
		call kernel_main

setup_gdt:
	mov WORD [gdtr], gdt_table_size
	mov DWORD [gdtr + 2], gdt_table
	lgdt [gdtr]
	mov bx, 0x10	; Offset to kernel data segment in GDT
	mov ds, bx	; Set Data Segment register
	mov bx, 0x18	; Offset to kernel stack segment in GDT
	mov ss, bx	; Set Stack Segment Registerr
	pop eax		; Get return address
	push 0x08	; Offset to kernel code segment in GDT
	push eax	; Push back return address
	retf		; Return (and set Code Segment register)

global stack_top
section .bss
	align 16
	stack_bottom:
	resb 16384			; reserve 16KB for stack
	stack_top:

	gdtr:
	resw 1	; limit
	resd 1	; base
