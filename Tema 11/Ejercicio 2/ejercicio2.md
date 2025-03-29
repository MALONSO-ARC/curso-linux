# Uso de ZRAM para Aumentar la Memoria Disponible

**Objetivo:** Configurar ZRAM como swap comprimido en RAM.

**Pasos:**
1. Carga el módulo y configura un dispositivo zram:
   ```bash
   sudo modprobe zram
   echo lz4 > /sys/block/zram0/comp_algorithm
   echo 128M | sudo tee /sys/block/zram0/disksize
   sudo mkswap /dev/zram0
   sudo swapon /dev/zram0
   ```
2. Verifica su uso:
   ```bash
   cat /proc/swaps
   free -h
   ```
3. Responde:
   - ¿Cuánto espacio de swap se activó?
   - ¿Qué beneficio tiene usar ZRAM frente a un archivo de swap en disco?