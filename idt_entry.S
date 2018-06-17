.globl breakpoint
breakpoint:
        // (1) Guardar registros.
        push %eax
        push %edx
        push %ecx
        
        // (2) Preparar argumentos de la llamada.
        movl $breakpoint_msg, %eax
        movl $14, %edx
        movl $0xB0, %ecx
        
        // (3) Invocar a vga_write2()
        call vga_write2
        
        // (4) Restaurar registros.
        pop %ecx
        pop %edx
        pop %eax
        
        // (5) Finalizar ejecución del manejador.
        iret

.data
breakpoint_msg:
        .asciz "Hello, breakpoint"

