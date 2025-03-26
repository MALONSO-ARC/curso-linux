---

# :file_folder: 7.5 Creación de RootFS con BusyBox

## :dart: Objetivo

Crear un sistema de archivos root mínimo (rootfs) utilizando BusyBox, adecuado para ejecutarse en entornos embebidos como QEMU o BeagleBone.

---

## Requisitos

- Sistema Linux con toolchain para ARM (ej: `arm-linux-gnueabihf-`)
- BusyBox (https://busybox.net)
- Utilidades: `cpio`, `gzip`, `sudo`, `make`

---

## Paso 1 - Configuración y compilación de BusyBox

1. Descarga BusyBox:
```bash
wget https://busybox.net/downloads/busybox-1.36.1.tar.bz2
```

2. Extrae y entra al directorio:
```bash
tar xjf busybox-1.36.1.tar.bz2
cd busybox-1.36.1
```

3. Configura con valores por defecto:
```bash
make defconfig
```

4. (Opcional) Activa la opción de instalar en directorio personalizado:
```bash
make menuconfig
# Selecciona: Settings → Installation path y selecciona el path ../rootfs
```

5. Compila:
```bash
make -j$(nproc) CROSS_COMPILE=arm-linux-gnueabihf-
```

---

## Paso 2 - Instalación en un directorio base

1. Instala los binarios en un directorio llamado `rootfs`:
```bash
make install
```

2. Verifica el contenido:
```bash
ls ../rootfs
```
Deberías ver directorios como `bin`, `sbin`, `usr`, etc.

---

## Paso 3 - Crear estructura de directorios esenciales

Asegúrate de que `rootfs` tenga:
```bash
mkdir -p ../rootfs/{dev,proc,sys,tmp,etc,lib,mnt}
```

Crea nodos de dispositivos (como `console` y `null`):
```bash
sudo mknod -m 622 ../rootfs/dev/console c 5 1
sudo mknod -m 666 ../rootfs/dev/null c 1 3
```

---

## Paso 4 - Generación de imagen de sistema de archivos

### Crear initramfs (cpio + gzip):
```bash
cd ../rootfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
```

### (Opcional) Convertir a uInitrd para U-Boot:
```bash
mkimage -A arm -T ramdisk -C gzip -n "Initramfs" \
  -d ../initramfs.cpio.gz ../uInitrd
```


