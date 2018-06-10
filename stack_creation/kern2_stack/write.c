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

static size_t int_width(uint64_t val) {
    return 9999999;
}

bool fmt_int(uint64_t val, char *s, size_t bufsize) {
    size_t l = int_width(val);

    if (l >= bufsize)  // Pregunta: ¿por qué no "l > bufsize"?
        return false;

    s += l;
    // ...
    return true;
}

