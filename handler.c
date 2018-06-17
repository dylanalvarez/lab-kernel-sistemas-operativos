#include "decls.h"
#include <stdbool.h>
#define LSHIFT '_'
#define RSHIFT '*'
//static unsigned ticks;
//
//void timer() {
//    if (++ticks == 15) {
//        vga_write("Transcurrieron 15 ticks", 20, 0x07);
//    }
//}

/**
 * Handler para el timer (IRQ0). Escribe un carácter cada segundo.
 */
static const uint8_t hz_ratio = 18;  // Default IRQ0 freq (18.222 Hz).
static bool rshift_is_pressed = false;
static bool lshift_is_pressed = false;

void timer() {
    static char chars[81];
    static unsigned ticks;
    static int8_t line = 20;
    static uint8_t idx = 0;

    if (++ticks % hz_ratio == 0) {
        chars[idx] = '.';
        chars[++idx] = '\0';
        vga_write(chars, line, 0x07);
    }

    if (idx >= sizeof(chars)) {
        line++;
        idx = 0;
    }
}

/**
 * Mapa de "scancodes" a caracteres ASCII en un teclado QWERTY.
 */
static char klayout[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8',                // 0-9
    '9', '0', 0,   0,   0,   0,   'q', 'w', 'e', 'r',                // 10-19
    't', 'y', 'u', 'i', 'o', 'p', 0,   0,   0,   0,                  // 20-29
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,   0,             // 30-40
    0,   LSHIFT,   0,   'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', // 41-52
    '-', RSHIFT};                                                    // 53-54
static char caps_lock_klayout[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8',                // 0-9
    '9', '0', 0,   0,   0,   0,   'Q', 'W', 'E', 'R',                // 10-19
    'T', 'Y', 'U', 'I', 'O', 'P', 0,   0,   0,   0,                  // 20-29
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0,   0,             // 30-40
    0,   LSHIFT,   0,   'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', // 41-52
    '-', RSHIFT};                                                    // 53-54
    
static const uint8_t KBD_PORT = 0x60;

bool shift_is_pressed() {
    return lshift_is_pressed || rshift_is_pressed;
}

/**
 * Handler para el teclado (IRQ1).
 *
 * Imprime la letra correspondiente por pantalla.
 */
void keyboard() {
    uint8_t code;
    static char chars[81];
    static uint8_t idx = 0;

    asm volatile("inb %1,%0" : "=a"(code) : "n"(KBD_PORT));

    if (code >= sizeof(klayout) || !klayout[code]) {
        // is unknown key or key release event
        if (klayout[code & ~0x80] == LSHIFT) {
            // is lshift key release event
            lshift_is_pressed = false;
        } else if (klayout[code & ~0x80] == RSHIFT) {
            // is rshift key release event
            rshift_is_pressed = false;
        }
        return;
    } else if (klayout[code] == LSHIFT) {
        // is lshift key press event
        lshift_is_pressed = true;
        return;
    } else if (klayout[code] == RSHIFT) {
        // is rshift key press event
        rshift_is_pressed = true;
        return;
    }

    if (idx == 80) {
        while (idx--)
            chars[idx] = ' ';
    }

    chars[idx++] = shift_is_pressed() ? caps_lock_klayout[code] : klayout[code];
    vga_write(chars, 19, 0x0A);
}

