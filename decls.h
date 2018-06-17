#ifndef KERN2_DECL_H
#define KERN2_DECL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// write.c (función de kern0-vga copiada no-static).
void vga_write(const char *s, int8_t linea, uint8_t color);

// igual a vga_write, pero toma sus parámetros directamente
// por registros
void __attribute__((regparm(3)))
vga_write2(const char *s, int8_t linea, uint8_t color);

// Escribe en ‘s’ el valor de ‘val’ en base 10 si su anchura
// es menor que ‘bufsize’. En ese caso devuelve true, caso de
// no haber espacio suficiente no hace nada y devuelve false.
bool fmt_int(uint64_t val, char *s, size_t bufsize);

void contador_run();

#endif

