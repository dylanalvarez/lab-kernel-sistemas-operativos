CFLAGS := -g -m32 -O1

kern2: boot.o kern2.o write.o
	ld -m elf_i386 -Ttext 0x100000 --entry _start $^ -o $@
	# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kern2 *.o core

.PHONY: clean
