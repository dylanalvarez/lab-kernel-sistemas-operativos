#include "decls.h"
#define LINE_LENGTH 80

void vga_write(const char *s, int8_t linea, uint8_t color) {
    volatile char *video = (volatile char*) 0xB8000;
    video += LINE_LENGTH * linea * 2;
    while (*s != 0) {
        *video++ = *s++;
        *video++ = color;
    }
}

