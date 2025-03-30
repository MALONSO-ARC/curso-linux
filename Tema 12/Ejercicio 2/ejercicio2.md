
## Depuración remota con `gdbserver`

### Objetivo:
Practicar la conexión entre el host y un sistema embebido simulado.

### Pasos:
1. Instala `gdbserver` si no está:
   ```bash
   sudo apt install gdbserver
   ```
2. Usa el binario del ejercicio anterior (`crash`).
3. En una terminal, simula el target:
   ```bash
   gdbserver :1234 ./crash
   ```
4. En otra terminal:
   ```bash
   gdb ./crash
   (gdb) target remote localhost:1234
   (gdb) continue
   ```