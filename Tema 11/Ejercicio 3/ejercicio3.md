# Creación de un Archivo de Swap Tradicional

**Objetivo:** Comparar swap en almacenamiento persistente vs swap en RAM comprimida.

**Requisitos Previos**  Instalar zram-tools ejecutando el comando ```sudo apt install zram-tools```

**Nota** La VM del curso ya tiene un fichero de swap ```/swapfile```. Por ello, el fichero que crearemos en el ejemplo se llama ```swapfile1```

**Pasos:**
1. Crea un archivo swap de 256MB:
   ```bash
   sudo dd if=/dev/zero of=/swapfile1 bs=1M count=256
   sudo chmod 600 /swapfile1
   sudo mkswap /swapfile1
   sudo swapon /swapfile1
   ```
2. Compara la salida de `free -h` con la anterior.
3. Prioriza swap en ZRAM:
   ```bash
   sudo swapoff /swapfile1
   sudo swapon -p 100 /dev/zram0
   sudo swapon -p 10 /swapfile1
   ```
4. Responde:
   - ¿Qué swap se usará primero y por qué?
   - ¿Cuándo es conveniente usar swap en disco en sistemas embebidos?