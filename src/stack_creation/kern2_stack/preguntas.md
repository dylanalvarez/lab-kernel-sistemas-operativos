**¿Qué significa "estar alineado"?**

Alinear datos significa situarlos en un offset de memoria igual a un múltiplo del tamaño de un *word*

**Mostrar la sintaxis de C/GCC para alinear a 32 bits el arreglo kstack anterior.**

Se debe setear el atributo (para el compilador GCC) 'aligned' de la variable, que especifica el mínimo alineamiento de la misma (en bytes, es decir en este caso, 4):

`unsigned char kstack[8192] __attribute__ ((aligned (4)));
`

**¿A qué valor se está inicializando kstack? ¿Varía entre la versión C y la versión ASM?**

(TODO)

**Explicar la diferencia entre las directivas .align y .p2align de as, y mostrar cómo alinear el stack del kernel a 4 KiB usando cada una de ellas.**

(TODO)
