CC				=	gcc
CFLAGS			=	-m32 -Wall -Wextra -Werror -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs
ifeq ($(BONUS), 1)
	CFLAGS		+= -D BONUS
endif
INCLUDES		=	-I includes/
C_SRCS			=	\
					kernel.c \
					display.c \
					io.c \
					utils.c \
					screen.c \
					$(addprefix interrupts/, \
						idt.c \
						pic.c \
						keyboard.c \
						interrupts.c \
					)

ASMC			=	nasm
ASMFLAGS		=	-f elf32
ASM_SRCS		=	\
					boot.s \
					interrupts.s

C_OBJS			=	$(addprefix C/,$(C_SRCS:.c=.o))
C_OBJS_DEPEND	=	${C_OBJS:.o=.d}
ASM_OBJS		=	$(addprefix ASM/,$(ASM_SRCS:.s=.o))
_OBJS			=	$(C_OBJS) $(ASM_OBJS)
OBJS			=	$(addprefix build/,$(_OBJS))

LD				=	ld
LD_FILE			=	linker.ld
LDFLAGS			=	-m elf_i386 -T $(LD_FILE)

KERNEL_BIN		=	iso/boot/kernel.bin
KERNEL_ISO		=	kernel.iso


all:	$(KERNEL_ISO)

$(KERNEL_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJS)

-include $(C_OBJS_DEPEND)
build/C/%.o: srcs/C/%.c
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CC) ${CFLAGS} -MMD -MF $(@:.o=.d) ${INCLUDES} -c $< -o $@


build/ASM/%.o: srcs/ASM/%.s
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(ASMC) ${ASMFLAGS} $< -o $@

$(KERNEL_ISO): $(KERNEL_BIN)
	grub-mkrescue -o $(KERNEL_ISO) iso

run: $(KERNEL_ISO)
	qemu-system-i386 -cdrom $(KERNEL_ISO)

clean:
	rm -rf build/

fclean: clean
	rm -rf $(KERNEL_BIN) $(KERNEL_ISO)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re
