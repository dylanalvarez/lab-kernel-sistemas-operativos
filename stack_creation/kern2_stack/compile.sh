# Compilar C y ASM
gcc -g -m32 -O1 -c kern2.c write.c boot.S

# Enlazar
ld -m elf_i386 -Ttext 0x100000 kern2.o write.o boot.o -o kern2
