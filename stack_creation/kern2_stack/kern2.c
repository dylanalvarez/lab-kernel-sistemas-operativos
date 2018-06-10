#include "decls.h"
#include "multiboot.h"
#include <string.h>

#define BUFLEN 256

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
}

