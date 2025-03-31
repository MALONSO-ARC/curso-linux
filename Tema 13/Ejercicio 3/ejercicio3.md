# Cifrado de partición con `cryptsetup`

**Objetivo:** Crear y montar una partición cifrada de forma segura.

**Nota** Instalar el paquete cryptsetup con el comando ```sudo apt install cryptsetup```

## Pasos:
1. **Crear imagen de disco:**
   ```bash
   dd if=/dev/zero of=disk.img bs=1M count=100
   losetup -fP disk.img
   losetup -a   # verifica el dispositivo asignado, por ejemplo /dev/loop0
   ```

2. **Formatear con LUKS:**
   ```bash
   cryptsetup luksFormat /dev/loop0
   cryptsetup open /dev/loop0 securepart
   ```

3. **Crear sistema de archivos:**
   ```bash
   mkfs.ext4 /dev/mapper/securepart
   mkdir /mnt/secure
   mount /dev/mapper/securepart /mnt/secure
   echo "Datos secretos" > /mnt/secure/secreto.txt
   umount /mnt/secure
   cryptsetup close securepart
   ```

4. **Reabrir y verificar:**
   ```bash
   cryptsetup open /dev/loop0 securepart
   mount /dev/mapper/securepart /mnt/secure
   cat /mnt/secure/secreto.txt
   ```