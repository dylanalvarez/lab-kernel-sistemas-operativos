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

**Explicar cómo se comporta strlcat(3) si, erróneamente, se declarase buf con tamaño 12. ¿Introduce algún error el código?**

Si no se modifica el argumento `size`, ocurre un buffer overflow (de hecho el kernel crashea). Esto se debe a que strlcat se permite a sí mismo escribir hasta `size` caracteres a partir del comienzo del string destino. Si se setea dicho argumento en 12, simplemente se conservarán los primeros 11 caracteres del string a imprimir por pantalla.

**Compilar el siguiente programa, y explicar por qué se imprimen dos líneas distintas, en lugar de la misma dos veces:**

```
#include <stdio.h>

static void printf_sizeof_buf(char buf[256]) {
    printf("sizeof buf = %zu\n", sizeof buf);
}

int main(void) {
    char buf[256];
    printf("sizeof buf = %zu\n", sizeof buf);
    printf_sizeof_buf(buf);
}
```
Los nombres de array no pueden ser argumentos en una función: al ser pasados a una función lo que se pasa es el puntero equivalente.

Entonces, por más que uno defina a su función como 'tomando un array de 256 caracteres', lo que es en realidad ese parámetro es la dirección del primer elemento de dicho array. Entonces, suele ser considerado como más claro siempre usar notación de punteros y no de arreglos al declarar funciones.

En este caso puntual, se imprime primero el tamaño del array de char `buf`, y luego el tamaño del puntero a char `buf`.