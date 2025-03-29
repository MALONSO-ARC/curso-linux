# Creación de un Archivo de Swap Tradicional

**Objetivo:** Comparar swap en almacenamiento persistente vs swap en RAM comprimida.

**Pasos:**
1. Crea un archivo swap de 256MB:
   ```bash
   sudo dd if=/dev/zero of=/swapfile bs=1M count=256
   sudo chmod 600 /swapfile
   sudo mkswap /swapfile
   sudo swapon /swapfile
   ```
2. Compara la salida de `free -h` con la anterior.
3. Prioriza swap en ZRAM:
   ```bash
   sudo swapoff /swapfile
   sudo swapon -p 100 /dev/zram0
   sudo swapon -p 10 /swapfile
   ```
4. Responde:
   - ¿Qué swap se usará primero y por qué?
   - ¿Cuándo es conveniente usar swap en disco en sistemas embebidos?