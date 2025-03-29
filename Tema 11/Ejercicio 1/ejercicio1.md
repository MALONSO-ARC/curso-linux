# Inspección del Uso de Memoria del Sistema

**Objetivo:** Comprender el uso de memoria física y virtual mediante herramientas del sistema.

**Pasos:**
1. Ejecuta los siguientes comandos y analiza su salida:
   ```bash
   free -h
   cat /proc/meminfo
   smem -r | sort -k 4 -h | tail
   ```
2. Responde:
   - ¿Cuánta memoria RAM total tiene el sistema?
   - ¿Qué procesos están usando más memoria residente (RSS)?
   - ¿Qué significa "buffers/cache" en la salida de `free`?

