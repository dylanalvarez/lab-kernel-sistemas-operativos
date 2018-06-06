QEMU := qemu-system-i386 -serial mon:stdio
KERN := kern2
BOOT := -kernel $(KERN)
CFLAGS := -g -m32 -O1 -ffreestanding -std=c99 -Wall -Wextra -Wpedantic -fasm

kern2: boot.o kern2.o write.o
	ld -m elf_i386 -Ttext 0x100000 --entry _start $^ -o $@
	# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kern2 *.o core
	
qemu: $(KERN)
	$(QEMU) $(BOOT)

qemu-gdb: $(KERN)
	$(QEMU) -kernel kern2 -S -gdb tcp:127.0.0.1:7508 $(BOOT)

gdb:
	gdb -q -s kern2 -n -ex 'target remote 127.0.0.1:7508' -tui

.PHONY: clean qemu qemu-gdb gdb
