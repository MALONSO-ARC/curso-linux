# Detección de fugas de memoria con Valgrind

## Objetivo:
Detectar y entender una fuga de memoria.

## Código:
```c
// leak.c
#include <stdlib.h>
int main() {
    char *buffer = malloc(100);
    return 0; // No se libera buffer
}
```

## Pasos:
1. Compilar:
   ```bash
   gcc -g -o leak leak.c
   ```
2. Ejecutar con Valgrind:
   ```bash
   valgrind --leak-check=full ./leak
   ```