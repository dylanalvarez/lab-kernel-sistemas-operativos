#include "decls.h"

#define COUNTLEN 20
#define TICKS (1ULL << 15)
#define DELAY(x) (TICKS << (x))
#define USTACK_SIZE 4096

static volatile char *const VGABUF = (volatile void *) 0xb8000;

static uintptr_t esp;
static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

extern void task_swap(uintptr_t *esp);
extern void task_exec(uintptr_t entry, uintptr_t stack);

static void yield() {
    if (esp) task_swap(&esp);
}

static void exit() {
    uintptr_t tmp = esp;
    esp = 0;
    if (tmp) task_swap(&tmp);
}

void contador_yield(unsigned lim, uint8_t linea, char color) {
    char counter[COUNTLEN] = {'0'};  // ASCII digit counter (RTL).

    while (lim--) {
        char *c = &counter[COUNTLEN];
        volatile char *buf = VGABUF + 160 * linea + 2 * (80 - COUNTLEN);

        unsigned p = 0;
        unsigned long long i = 0;

        while (i++ < DELAY(6))  // Usar un entero menor si va demasiado lento.
            ;

        while (counter[p] == '9') {
            counter[p++] = '0';
        }

        if (!counter[p]++) {
            counter[p] = '1';
        }

        while (c-- > counter) {
            *buf++ = *c;
            *buf++ = color;
        }

        yield();
    }
}

void contador_run() {
    // Configurar stack1 y stack2 con los valores apropiados.
    uintptr_t *a = (uintptr_t *) &stack1[USTACK_SIZE - 1];
    uintptr_t *b = (uintptr_t *) &stack2[USTACK_SIZE - 1];

    // contador_yield(100, 0, 0x2F);
    a -= 3;
    a[0] = 100;     // 3 function arguments
    a[1] = 0;
    a[2] = 0x2F;

    // contador_yield(100, 1, 0x4F);
    b -= 9;
    b[0] = 0;       // space needed for later
    b[1] = 0;       // popping the 4 callee
    b[2] = 0;       // saved registers
    b[3] = 0;


    b[4] = (uintptr_t) contador_yield; // task_swap will return
                                       // to this addr
    b[5] = (uintptr_t) exit;           // contador_yield will
                                       // return to this addr
    b[6] = 50;     // 3 function arguments
    b[7] = 1;
    b[8] = 0x4F;

    // Actualizar la variable estática ‘esp’ para que apunte
    // al del segundo contador.
    esp = (uintptr_t) b;

    // Lanzar el primer contador con task_exec.
    task_exec((uintptr_t) contador_yield, (uintptr_t) a);
}
