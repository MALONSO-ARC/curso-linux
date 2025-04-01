# Analizar el rendimiento de una aplicación con `perf`

**Objetivo:** Generar un perfil de una aplicación simple y analizarlo con `perf`.

## Pasos:
1. Crea el siguiente programa:
```c
// cpu_test.c
#include <stdio.h>
int main() {
    volatile int x = 0;
    for (int i = 0; i < 500000000; ++i) x++;
    printf("Resultado: %d\n", x);
    return 0;
}
```
2. Compila con:
```bash
gcc -O2 -g cpu_test.c -o cpu_test
```
3. Ejecuta:
```bash
perf stat ./cpu_test
```
4. Luego:
```bash
perf record ./cpu_test
perf report
```

**Preguntas:**
- ¿Qué función consumió más tiempo?
- ¿Cuántos ciclos/instrucciones se ejecutaron?