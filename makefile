AC		:= nasm
AC_FLAGS	:= -f elf32

CC		:= gcc
CC_FLAGS	:= -nostdlib -ffreestanding -O0 -Wall -Wextra -fno-pie -m32

TARGET_DIR	:= build
HEADERS	:= include/kernel.h include/drivers/mlprp_vga_text.h
OBJS		:= $(TARGET_DIR)/boot.o $(TARGET_DIR)/entry.o $(TARGET_DIR)/kernel.o $(TARGET_DIR)/vga.o

.PHONY: all compile build make_disk clean

all: compile build make_disk

compile:
	@echo "compiling objects..."
	$(AC) $(AC_FLAGS) -o $(TARGET_DIR)/boot.o boot/boot.asm
	$(AC) $(AC_FLAGS) -o $(TARGET_DIR)/entry.o kernel/entry.asm
	$(CC) $(CC_FLAGS) -c kernel/kernel.c -o $(TARGET_DIR)/kernel.o
	$(CC) $(CC_FLAGS) -c kernel/drivers/mlprp_vga_text.c -o $(TARGET_DIR)/vga.o

build:
	@echo "building..."
	$(CC) $(CC_FLAGS) -lgcc -T linker.ld -o $(TARGET_DIR)/boot.bin $(HEADERS) $(OBJS)

make_disk:
	@echo "generating virtual floppy..."
	dd if=/dev/zero of=$(TARGET_DIR)/floppy.img bs=1024 count=1440
	dd if=$(TARGET_DIR)/boot.bin of=$(TARGET_DIR)/floppy.img seek=0 count=1024 conv=notrunc
	qemu-system-x86_64 -drive format=raw,file=$(TARGET_DIR)/floppy.img
	
clean:
	@echo "cleaning build directory..."
	rm -rf build/*
