
# Depuración de un crash en una aplicación C con GDB

## Objetivo:
Identificar la causa de un fallo por segmentación en una aplicación C.

## Pasos:
1. Crea el siguiente programa con fallo deliberado:
   ```c
   // crash.c
   #include <stdio.h>
   int main() {
       int *ptr = NULL;
       *ptr = 42;
       return 0;
   }
   ```
2. Compílalo con símbolos de depuración:
   ```bash
   gcc -g -o crash crash.c
   ```
3. Ejecuta y observa el fallo:
   ```bash
   ./crash
   ```
4. Usa GDB para analizar:
   ```bash
   gdb ./crash
   (gdb) run
   (gdb) bt
   ```



---


   ```

---



---

> Todos los ejercicios deben ejecutarse en una terminal de Ubuntu 22.04 y requieren permisos de usuario estándar (salvo donde se indique `sudo`).

