.align 4
.text
.globl task_exec
task_exec:
        // void task_exec(uintptr_t entry, uintptr_t stack);
        // Preámbulo estándar
        push %ebp
        movl %esp, %ebp
        
        push %ebx           // Guardo ebx (callee saved register)
        movl %esp, %ebx     // Lo uso para guardar tope de stack
        movl 12(%ebp), %esp // Cambio de stack
        call *8(%ebp)       // Llamo a entry
        movl %ebx, %esp     // Restauro el tope de stack
        pop %ebx            // Restauro ebx

        leave
        ret

