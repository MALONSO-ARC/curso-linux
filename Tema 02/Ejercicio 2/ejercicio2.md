## 🧪 Ejercicio 1: Identificar y montar correctamente la eMMC y la microSD

### Objetivo
Comprender cómo Linux detecta los dispositivos de almacenamiento (eMMC y microSD) y practicar el montaje manual.

### Pasos
1. Arrancar la BeagleBone Black desde la eMMC (sin tarjeta SD).
2. Ejecutar los siguientes comandos para identificar el dispositivo de arranque:
   ```bash
   lsblk
   mount | grep mmc
   ```
3. Insertar una tarjeta microSD previamente formateada en `ext4`.
4. Identificar el nuevo dispositivo (`/dev/mmcblk0` o `/dev/mmcblk1`).
5. Montar la tarjeta en `/mnt/sd`:
   ```bash
   mkdir -p /mnt/sd
   mount /dev/mmcblk0p1 /mnt/sd
   ```



