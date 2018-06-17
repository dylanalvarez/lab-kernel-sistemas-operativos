#define PIC1 0x20
#define ACK_IRQ 0x20

.globl ack_irq
ack_irq:
        // Indicar que se manejó la interrupción.
        movl $ACK_IRQ, %eax
        outb %al, $PIC1
        iret


//////////////////////////////////
.globl divzero
divzero:
        // Guardar registros.
        push %eax
        push %edx
        push %ecx
        
        // Preparar argumentos de la llamada.
        movl $divzero_msg, %eax
        movl $17, %edx
        
        // Invocar a vga_write_cyan()
        call vga_write_cyan
        
        // Incremento %ebx
        inc %ebx
        
        // Restaurar registros.
        pop %ecx
        pop %edx
        pop %eax
        
        jmp ack_irq
        
        
//////////////////////////////////
.globl keyboard_asm
keyboard_asm:
        // Guardar registros.
        push %eax
        push %edx
        push %ecx
        
        call keyboard
        
        // Restaurar registros.
        pop %ecx
        pop %edx
        pop %eax
        
        jmp ack_irq


//////////////////////////////////
.globl timer_asm
timer_asm:
        // Guardar registros.
        push %eax
        push %edx
        push %ecx
        
        call timer
        
        // Restaurar registros.
        pop %ecx
        pop %edx
        pop %eax
        
        jmp ack_irq


//////////////////////////////////
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
divzero_msg:
        .asciz "Se divide por ++ebx"

