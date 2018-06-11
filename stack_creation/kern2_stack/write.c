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

char digit_to_char(uint64_t digit) {
    if (digit == 0) return '0';
    if (digit == 1) return '1';
    if (digit == 2) return '2';
    if (digit == 3) return '3';
    if (digit == 4) return '4';
    if (digit == 5) return '5';
    if (digit == 6) return '6';
    if (digit == 7) return '7';
    if (digit == 8) return '8';
    if (digit == 9) return '9';
    return '_';
}

static size_t int_width(uint64_t n) {
    if (n < 10u) return 1;
    if (n < 100u) return 2;
    if (n < 1000u) return 3;
    if (n < 10000u) return 4;
    if (n < 100000u) return 5;
    if (n < 1000000u) return 6;
    if (n < 10000000u) return 7;
    if (n < 100000000u) return 8;
    if (n < 1000000000u) return 9;
    if (n < 10000000000u) return 10;
    if (n < 100000000000u) return 11;
    if (n < 1000000000000u) return 12;
    if (n < 10000000000000u) return 13;
    if (n < 100000000000000u) return 14;
    if (n < 1000000000000000u) return 15;
    if (n < 10000000000000000u) return 16;
    if (n < 100000000000000000u) return 17;
    if (n < 1000000000000000000u) return 18;
    if (n < 10000000000000000000u) return 19;
    return 20; // 2^64 = 18446744073709551616: 20 characters
}

bool fmt_int(uint64_t val, char *s, size_t bufsize) {
    size_t _strlen = int_width(val);

    if (_strlen >= bufsize)
    // Pregunta: ¿por qué no "_strlen > bufsize"?
    // Respuesta: porque hay que considerar el 0 al final.
        return false;

    s += _strlen;
    *s = 0;
    for (size_t i = 0; i < _strlen; i++) {
        s -= 1;
        *s = digit_to_char(val % 10);
        val /= 10;
    }
    return true;
}

