#include "decls.h"
#include "sched.h"
#include "multiboot.h"
#include <string.h>

#define BUFLEN 256
#define USTACK_SIZE 4096

extern void two_stacks();
extern void task_exec(uintptr_t entry, uintptr_t stack);

void print_cmdline(const multiboot_info_t *mbi) {
    if (mbi->flags & (1 << 2)) { // bit 2 of flags is set
        char buf[BUFLEN] = "cmdline: ";
        // Aquí usar strlcat() para concatenar cmdline a buf.
        strlcat(buf, (char *) mbi->cmdline, BUFLEN);
        // Forma de hacer lo mismo con strncat:
        //strncat(buf, (char *) mbi->cmdline, BUFLEN - strlen(buf) - 1);
        vga_write(buf, 9, 0x07);
    }
}

void print_memory_size(const multiboot_info_t *mbi) {
        if (mbi->flags & 1) { // bit 1 of flags is set
        char mem[256] = "Physical memory: ";
        char tmp[64] = "";

        uint32_t mem_total = (mbi->mem_lower + mbi->mem_upper) >> 10; // KiB to MiB

        if (fmt_int(mem_total, tmp, sizeof tmp)) {
            strlcat(mem, tmp, sizeof mem);
            strlcat(mem, "MiB total", sizeof mem);
        }

        if (fmt_int(mbi->mem_lower, tmp, sizeof tmp)) {
            strlcat(mem, " (", sizeof mem);
            strlcat(mem, tmp, sizeof mem);
            strlcat(mem, "KiB base", sizeof mem);
            if (fmt_int(mbi->mem_upper, tmp, sizeof tmp)) {
                strlcat(mem, ", ", sizeof mem);
                strlcat(mem, tmp, sizeof mem);
                strlcat(mem, "KiB extended)", sizeof mem);
            } else {
                strlcat(mem, ")", sizeof mem);
            }
        }

        vga_write(mem, 10, 0x07);
    }
}

static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

void two_stacks_c() {
    // Inicializar al *tope* de cada pila.
    uintptr_t *a = (uintptr_t *) &stack1[USTACK_SIZE - 1];
    uintptr_t *b = (uintptr_t *) &stack2[USTACK_SIZE - 1];

    // Preparar, en stack1, la llamada:
    // vga_write("vga_write() from stack1", 15, 0x57);

    *(--a) = 0x57;
    *(--a) = 15;
    *(--a) = (uintptr_t) "vga_write() from stack1";

    // Preparar, en s2, la llamada:
    // vga_write("vga_write() from stack2", 16, 0xD0);

    b -= 3;
    b[0] = (uintptr_t) "vga_write() from stack2";
    b[1] = 16;
    b[2] = 0xD0;

    // Primera llamada usando task_exec().
    task_exec((uintptr_t) vga_write, (uintptr_t) a);

    // Segunda llamada con ASM directo. Importante: no
    // olvidar restaurar el valor de %esp al terminar, y
    // compilar con: -fasm -fno-omit-frame-pointer.
    asm("push %%ebx;"       // Guardo ebx (callee saved register)
        "movl %%esp, %%ebx;"// Lo uso para guardar tope de stack
        "movl %0, %%esp;"   // Cambio de stack
        "call *%1;"         // Llamo a vga_write
        "movl %%ebx, %%esp;"// Restauro el tope de stack
        "pop %%ebx;"        // Restauro ebx
        : /* no outputs */
        : "r"(b), "r"(vga_write));
}

static void contador1() {
    contador_yield(50000000, 2, 0x2F);
}

static void contador2() {
    contador_yield(50000000, 3, 0x6F);
}

static void contador3() {
    contador_yield(50000000, 4, 0x4F);
}

void contador_spawn() {
    spawn(contador1);
    spawn(contador2);
    spawn(contador3);
}

void kmain(const multiboot_info_t *mbi) {
    int8_t linea;
    uint8_t color;

    vga_write("kern2 loading.............", 8, 0x70);
    two_stacks();
    two_stacks_c();
    print_cmdline(mbi);
    print_memory_size(mbi);

    sched_init();
    // Código ejercicio kern2-idt.
    idt_init();
    irq_init();
    asm("int3");

    asm("div %4"
    : "=a"(linea), "=c"(color)
    : "0"(18), "1"(0xE0), "b"(1), "d"(0));
    vga_write2("Funciona vga_write2?", linea, color);
    //vga_write2("Funciona vga_write2?", 18, 0xE0);
    contador_spawn();

    contador_run();
}

