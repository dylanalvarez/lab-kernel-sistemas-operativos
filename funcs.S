.globl vga_write2
vga_write2:
        push %ebp
        movl %esp, %ebp

        push %ecx
        push %edx
        push %eax
        call vga_write

        leave
        ret

/*
objdump -d kern2.o

00000292 <kmain>:
 292:	55                   	push   %ebp
 293:	89 e5                	mov    %esp,%ebp
 295:	83 ec 0c             	sub    $0xc,%esp
 298:	6a 70                	push   $0x70
 29a:	6a 08                	push   $0x8
 29c:	68 59 00 00 00       	push   $0x59
 2a1:	e8 fc ff ff ff       	call   2a2 <kmain+0x10>
 2a6:	e8 fc ff ff ff       	call   2a7 <kmain+0x15>
 2ab:	e8 fc ff ff ff       	call   2ac <kmain+0x1a>
 2b0:	b9 e0 00 00 00       	mov    $0xe0,%ecx
 2b5:	ba 12 00 00 00       	mov    $0x12,%edx
 2ba:	b8 74 00 00 00       	mov    $0x74,%eax
 2bf:	e8 fc ff ff ff       	call   2c0 <kmain+0x2e>
 2c4:	83 c4 10             	add    $0x10,%esp
 2c7:	c9                   	leave  
 2c8:	c3                   	ret
 
los 4 call corresponden a (en orden de aparición):
 vga_write
 two_stacks
 two_stacks_c
 vga_write2
*/

