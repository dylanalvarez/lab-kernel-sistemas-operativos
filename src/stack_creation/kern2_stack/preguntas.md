**¿Qué significa "estar alineado"?**

Alinear datos significa situarlos en un offset de memoria igual a un múltiplo del tamaño de un *word*.

**Mostrar la sintaxis de C/GCC para alinear a 32 bits el arreglo kstack anterior.**

Se debe setear el atributo (para el compilador GCC) 'aligned' de la variable, que especifica el mínimo alineamiento de la misma (en bytes, es decir en este caso, 4):

`unsigned char kstack[8192] __attribute__ ((aligned (4)));
`

**¿A qué valor se está inicializando kstack? ¿Varía entre la versión C y la versión ASM?**

`kstack`, tanto en la versión C como en la versión ASM, se inicializa con ceros.

En la versión C se debe a que, según el estándar, toda variable estática o global no inicializada explícitamente se inicializa con ceros.

En la versión ASM se debe a que la directiva `.space size, fill` asume que 'fill' es 0 si no se lo explicita.

**Explicar la diferencia entre las directivas .align y .p2align de as, y mostrar cómo alinear el stack del kernel a 4 KiB usando cada una de ellas.**

`.align alignment, fill_value, max_bytes_skipped` agrega padding antes de la instrucción/dato siguiente: 'fill_value' indica el valor con que rellenar los bytes de padding (default: 0 o noop según corresponda), 'max_bytes_skipped' es el máximo numero de bytes a rellenar (si se excede, no se agrega padding).

El problema es que 'alignment' es ambiguo: en algunos sistemas es el número de bytes de que tiene que ser múltiple la localización, y en otros es el número de bits '0' que debe tener dicha localización en los bits menos significativos (el primer valor es 2 elevado al segundo valor).

`.balign` (alinear por bytes) hace lo primero, y `.p2align` (alinear por potencia de 2) hace lo segundo, en todos los sistemas, por lo que solucionan este problema haciendo explícita la interpretación de 'alignment'.

Ejemplos:

```
.data
kstack:
    .align 4096
    .space 8192
```

```
.data
kstack:
    .p2align 12
    .space 8192
```
