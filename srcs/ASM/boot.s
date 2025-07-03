bits 32

%define MAGIC 0x1BADB002       
%define MULTIBOOT_FLAGS 0  
%define CHECKSUM - (MAGIC + MULTIBOOT_FLAGS)

section .multiboot
	dd MAGIC			; Multiboot magic number
	dd MULTIBOOT_FLAGS	; Multiboot flags
	dd CHECKSUM			; Checksum for the multiboot header

section .text
	global start

	extern kernel_main

	start:
		mov esp, stack_top
		call kernel_main

global stack_top
section .bss
	align 16
	stack_bottom:
	resb 16384			; reserve 16KB for stack
	stack_top:
