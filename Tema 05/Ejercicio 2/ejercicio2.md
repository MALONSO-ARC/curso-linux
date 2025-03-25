Carga del Kernel y DTB desde tarjeta SD

### Objetivo
Configurar U-Boot para cargar el kernel y el Device Tree desde la tarjeta SD.

### Requisitos
- Archivos `zImage`,  `dtb` y `initramfs` en una partición de la tarjeta SD.

### Pasos

1. **Copiar los archivos al sistema de archivos en la SD**:
```bash
cp zImage am335x-boneblack.dtb /media/$USER/BOOT/
```

2. **En la consola de U-Boot**:
### 1. Seleccionar el dispositivo MMC (tarjeta SD)
```bash
mmc dev 0
```

### 2. Verificar particiones (opcional)
```bash
mmc part
```

### 3. Cargar el kernel desde la partición ext4
```bash
ext4load mmc 0:2 0x82000000 /boot/zImage
```

### 4. Cargar el Device Tree
```bash
ext4load mmc 0:2 0x83000000 /boot/am335x-boneblack.dtb
```

### 5. (Opcional) Cargar el initramfs
```bash
ext4load mmc 0:2 0x84000000 /boot/initrd.img
```

### 6. Configurar la línea de comandos del kernel
```bash
setenv bootargs console=ttyO0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 rw
```

### 7. Iniciar el kernel

#### Si **NO** se usa initramfs:
```bash
bootz 0x82000000 - 0x83000000
```

#### Si **SÍ** se usa initramfs:
```bash
bootz 0x82000000 0x84000000 0x83000000
```

---

## 🧠 Notas
- `ext4load` requiere que U-Boot tenga soporte para sistemas ext4 habilitado.
- `bootz` es para kernels en formato `zImage`. Si usas `uImage`, usa `bootm`.
- Puedes automatizar esto con un script en una variable `bootcmd` y ejecutar `saveenv`.


## **Opcional: Automatizar el arranque**:
```bash
setenv bootcmd 'ext4load mmc 0:1 0x82000000 zImage; ext4load mmc 0:1 0x83000000 am335x-boneblack.dtb; ext4load mmc 0:2 0x84000000 /boot/initrd.img; bootz 0x82000000 0x84000000 0x83000000'
saveenv
```