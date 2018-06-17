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

.globl task_swap
task_swap:
        // Pone en ejecución la tarea cuyo stack está en ‘*esp’,
        // cuyo valor se intercambia por el valor actual de %esp.
        // Guarda y restaura todos los callee-called registers.
        // void task_swap(uintptr_t *esp);
        
        push %ebx
        push %esi
        push %edi
        push %ebp
        
        // Como no tengo el 'ancla' que es ebp, tengo que
        // determinar el offset para el parametro de la
        // funcion respecto de esp:
        // los offset 0, 4, 8 y 12 corresponden a los 4 pushes
        // de arriba, el offset 16 corresponde a la dirección de
        // retorno y finalmente el 20 al parámetro buscado.
        movl %esp, %eax     // esp to eax ('saved esp')
        movl 20(%esp), %ecx // given pointer to ecx
        movl (%ecx), %esp   // pointer's content to esp
        movl %eax, (%ecx)   // 'saved esp' to pointer's content
        
        pop %ebp
        pop %edi
        pop %esi
        pop %ebx

        ret

