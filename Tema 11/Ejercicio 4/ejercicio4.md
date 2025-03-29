# Observación de la Asignación Dinámica con `malloc()`

**Objetivo:** Ver cómo se asigna y libera memoria desde un programa en C.

**Pasos:**
1. Crea un archivo `malloc_test.c` con el siguiente contenido:
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>

   int main() {
       printf("PID: %d\n", getpid());
       getchar(); // pausa para inspección

       char *buffer = malloc(100 * 1024 * 1024); // 100MB
       if (!buffer) {
           perror("malloc");
           return 1;
       }
       getchar(); // pausa para inspección

       free(buffer);
       getchar();
       return 0;
   }
   ```
2. Compila y ejecuta:
   ```bash
   gcc malloc_test.c -o malloc_test
   ./malloc_test
   ```
3. En otra terminal, inspecciona el uso de memoria con `top` o `smem`.
4. Responde:
   - ¿Cuándo se ve el aumento de uso de memoria?
   - ¿Después de `free()`, se libera inmediatamente la memoria?