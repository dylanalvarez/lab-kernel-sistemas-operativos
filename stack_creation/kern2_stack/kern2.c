#include "decls.h"
#include "multiboot.h"
#include "lib/string.h"

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);
       
    if (mbi->flags & (1 << 2)) { // bit 2 of flags is set
        char buf[256] = "cmdline: ";
        // Aquí usar strlcat() para concatenar cmdline a buf.
        strlcat(buf, (char *) mbi->cmdline, 256);
        vga_write(buf, 9, 0x07);
    }
}

