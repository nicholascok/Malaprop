all:
	nasm -f elf32 -o boot.o boot/boot.asm
	nasm -f elf32 -o entry.o kernel/entry.asm
	gcc -fno-pie -m32 -c kernel/kernel.c -o kernel.o -nostdlib -ffreestanding -O2 -Wall -Wextra
	gcc -fno-pie -m32 -c kernel/drivers/mlprp_vga.c -o vga.o -nostdlib -ffreestanding -O2 -Wall -Wextra
	gcc -fno-pie -T linker.ld -o boot.bin -ffreestanding -O2 -nostdlib include/kernel.h include/drivers/mlprp_vga.h boot.o entry.o kernel.o vga.o -lgcc -m32
#	qemu-system-x86_64 -fda boot.bin -boot a -m 100M

#	nasm -f bin -o boot.bin boot/boot.asm
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	dd if=boot.bin of=floppy.img seek=0 count=64 conv=notrunc
	qemu-system-x86_64 -drive format=raw,file=floppy.img
