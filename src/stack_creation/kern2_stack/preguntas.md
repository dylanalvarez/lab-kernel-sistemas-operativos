**¿Qué significa "estar alineado"?**

Alinear datos significa situarlos en un offset de memoria igual a un múltiplo del tamaño de un *word*

**Mostrar la sintaxis de C/GCC para alinear a 32 bits el arreglo kstack anterior.**

Se debe setear el atributo (para el compilador GCC) 'aligned' de la variable, que especifica el mínimo alineamiento de la misma (en bytes, es decir en este caso, 4):

`unsigned char kstack[8192] __attribute__ ((aligned (4)));
`

**¿A qué valor se está inicializando kstack? ¿Varía entre la versión C y la versión ASM?**

`kstack`, tanto en la versión C como en la versión ASM, se inicializa con ceros.

En la versión C se debe a que, según el estándar, toda variable estática o global no inicializada explícitamente se inicializa con ceros.

En la versión ASM se debe a que la directiva `.space size , fill` asume que 'fill' es 0 si no se lo explicita.

**Explicar la diferencia entre las directivas .align y .p2align de as, y mostrar cómo alinear el stack del kernel a 4 KiB usando cada una de ellas.**

(TODO)
