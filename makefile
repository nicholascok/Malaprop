all:
	nasm -f elf32 -o boot.bin boot/boot.asm
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	dd if=boot.bin of=floppy.img seek=0 count=64 conv=notrunc
	qemu-system-x86_64 -drive format=raw,file=floppy.img
