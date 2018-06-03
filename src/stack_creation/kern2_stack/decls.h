#ifndef KERN2_DECL_H
#define KERN2_DECL_H

#include <stdint.h>

// write.c (función de kern0-vga copiada no-static).
void vga_write(const char *s, int8_t linea, uint8_t color);

#endif

