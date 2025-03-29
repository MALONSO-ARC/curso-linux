# Uso de Valgrind para Detectar Fugas de Memoria

**Objetivo:** Detectar y corregir errores comunes de asignación dinámica.

**Pasos:**
1. Escribe un archivo `leak.c` con:
   ```c
   #include <stdlib.h>

   int main() {
       char *p = malloc(100);
       p[0] = 'A';
       return 0;
   }
   ```
2. Compila sin optimizaciones:
   ```bash
   gcc -g leak.c -o leak
   ```
3. Ejecuta con valgrind:
   ```bash
   valgrind --leak-check=full ./leak
   ```
4. Corrige el programa agregando `free(p);` y vuelve a ejecutar.
5. Responde:
   - ¿Qué tipo de advertencias genera Valgrind?
   - ¿Cómo afecta una fuga de memoria en un sistema embebido?