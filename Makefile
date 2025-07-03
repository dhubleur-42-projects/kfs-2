ISO_NAME	=	kfs.iso
KERNEL_NAME	=	build/kfs.elf

C_SRCS		= 	\
				kernel.c \
				print_stack.c \
				strings.c \
				vga.c

ASM_SRCS	=	\
				boot.s

_OBJS		=	${C_SRCS:.c=.o} ${ASM_SRCS:.s=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CC			=	clang
CFLAGS		=   -c -target i386-unknown-elf -fno-exceptions -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -masm=intel -g3
INCLUDE		=	-I includes/

ASMC		= nasm
ASMFLAGS	= -felf32

LINKER		= clang
LINKERFLAGS	= -target i386-unknown-elf -fuse-ld=lld -nostdlib -static -fno-exceptions -fno-builtin -fno-stack-protector -Wl,--build-id=none -T configs/linker.ld

QEMU		= qemu-system-i386
QEMU_ARGS	= -cdrom ${ISO_NAME}

all		:	$(ISO_NAME)

build/%.o	:	srcs/C/%.c
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CC) ${CFLAGS} -MMD -MF $(@:.o=.d) ${INCLUDE} -c $< -o $@

build/%.o	:	srcs/ASM/%.s
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(ASMC) $(ASMFLAGS) $< -o $@

$(KERNEL_NAME)	:	$(OBJS)
	$(LINKER) $(LINKERFLAGS) $(OBJS) -o $(KERNEL_NAME)

$(ISO_NAME)		:	$(KERNEL_NAME)
	mkdir -p build/isodir/boot/grub
	cp ${KERNEL_NAME} build/isodir/boot/
	cp configs/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${ISO_NAME} build/isodir

-include $(OBJS_DEPEND)

clean	:
	rm -Rf build/

fclean	:	clean
	rm -f ${ISO_NAME}

re		:	fclean
			make ${ISO_NAME}

boot	:	${ISO_NAME}
	qemu-system-i386 -s -S -cdrom ${ISO_NAME}

.PHONY	:	all clean fclean re boot
