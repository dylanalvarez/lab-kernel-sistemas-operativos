#include "decls.h"
#include "multiboot.h"
#include <string.h>

#define BUFLEN 256

extern void two_stacks();

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    if (mbi->flags & (1 << 2)) { // bit 2 of flags is set
        char buf[BUFLEN] = "cmdline: ";
        // Aquí usar strlcat() para concatenar cmdline a buf.
        strlcat(buf, (char *) mbi->cmdline, BUFLEN);
        // Forma de hacer lo mismo con strncat:
        //strncat(buf, (char *) mbi->cmdline, BUFLEN - strlen(buf) - 1);
        vga_write(buf, 9, 0x07);
    }
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

        // vga_write(mem, 10, 0x07);
        two_stacks();
    }
}
