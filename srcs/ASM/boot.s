bits 32

%define MAGIC 0x1BADB002       
%define MULTIBOOT_FLAGS 0  
%define CHECKSUM - (MAGIC + MULTIBOOT_FLAGS)

extern gdt_init

section .multiboot
	dd MAGIC			; Multiboot magic number
	dd MULTIBOOT_FLAGS	; Multiboot flags
	dd CHECKSUM			; Checksum for the multiboot header

section .text
	global start

	extern kernel_main

	start:
		mov esp, stack_top_init
		call gdt_init
		call kernel_main

section .bss
	align 16
	stack_bottom_init:
    resb 4096
	stack_top_init: