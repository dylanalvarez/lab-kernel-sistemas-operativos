#include "decls.h"
#include "interrupts.h"

static struct IDTR idtr;
static struct Gate idt[256];
extern void breakpoint();

void idt_init() {
  // (1) Instalar manejadores ("interrupt service routines").
  idt_install(T_BRKPT, breakpoint);

  idtr.base = (uintptr_t) &idt;   // write where the IDT is store in memory
  idtr.limit = (sizeof (struct Gate) * 256) - 1; // set the limit of the IDT

  asm("lidt %0" : : "m"(idtr));
}


// Multiboot siempre define "8" como el segmento de código.
// (Ver campo CS en `info registers` de QEMU.)
static const uint8_t KSEG_CODE = 8;

// Identificador de "Interrupt gate de 32 bits" (ver IA32-3A,
// tabla 6-2: IDT Gate Descriptors).
static const uint8_t STS_IG32 = 0xE;

void idt_install(uint8_t n, void (*handler)(void)) {
    uintptr_t addr = (uintptr_t) handler;

    idt[n].rpl = 0;
    idt[n].type = STS_IG32;
    idt[n].segment = KSEG_CODE;

    idt[n].off_15_0 = addr & 0xFFFF;
    idt[n].off_31_16 = addr >> 16;

    idt[n].present = 1;
}
