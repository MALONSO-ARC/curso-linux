# Análisis de un core dump

## Objetivo:
Analizar una aplicación caída usando el volcado de memoria.

## Pasos:
1. Habilita la creación de core dumps:
   ```bash
   ulimit -c unlimited
   ```
2. Ejecuta la aplicación `crash` para que genere un `core`.
3. Encuentra el archivo `core` (usualmente `core`, `core.<pid>` o en `/var/crash`).
4. Analízalo con:
   ```bash
   gdb ./crash core
   (gdb) bt
   ```