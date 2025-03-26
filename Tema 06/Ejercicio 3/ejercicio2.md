

## :toolbox: Requisitos

- BeagleBone Black física o emulada (QEMU opcional)
- Host Ubuntu 22.04 (o similar)
- Toolchain para ARM (ej. `arm-linux-gnueabihf-`)
- Conexión serial o consola UART (o terminal emulado)
- Repositorio del kernel BeagleBone: https://github.com/beagleboard/linux.git

---

## :repeat: Pasos

### 1. Clonar el código fuente del kernel

```bash
git clone --depth=1 https://github.com/beagleboard/linux.git -b 5.10 kernel-bbb
cd kernel-bbb
```

### 2. Configurar entorno de compilación

```bash
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
```

### 3. Cargar configuración base para AM335x

```bash
make bb.org_defconfig
```

### 4. Lanzar herramienta de configuración del kernel

```bash
make menuconfig
```

Dentro del menú interactivo:

#### Deshabilita drivers innecesarios:

Ruta: `Device Drivers`

- Desactiva:
  - Sound card support
  - Bluetooth subsystem support
  - Graphics support (si no usas pantalla HDMI)
  - Multimedia support
  - Staging drivers no utilizados
  - Soporte a Wi-Fi/Bluetooth USB si no se usan

#### Reduce soporte de sistemas de archivos:

Ruta: `File systems`

- Deja solo lo necesario (ej. ext4)
- Desactiva:
  - NTFS
  - Btrfs
  - F2FS
  - JFS, etc.

#### Ajuste opcional:

Ruta: `Kernel Features`

- Comprimir kernel con: `LZ4` o `XZ` (reduce el tamaño de `zImage`)

Guarda y sal.

---

### 5. Compilar el kernel

```bash
make -j$(nproc) zImage
```

### 6. Medir el tamaño resultante

```bash
ls -lh arch/arm/boot/zImage
```

Compara con la versión original sin cambios (opcional).

---

### 7. Copiar al sistema de arranque (U-Boot)

```bash
# En U-Boot:
ext4load mmc 0:1 0x80200000 zImage
ext4load mmc 0:1 0x88000000 am335x-boneblack.dtb
ext4load mmc 0:1 0x88000000 initrd.img-4.19.94-ti-r42 
setenv bootargs console=ttyO0,115200n8 bone_capemgr.uboot_capemgr_enabled=1 root=/dev/mmcblk1p1 ro rootfstype=ext4 rootwait coherent_pool=1M net.ifnames=0 lpj=1990656 rng_core.default_quality=100 quiet

bootz 0x80200000 0x88080000 0x88000000
```

---

## :mag_right: Verificación

- El sistema debe arrancar correctamente.
- Usa `dmesg` y `lsmod` para verificar drivers activos.
- Comparar tamaños de `zImage` antes y después.

---

## :thinking: Preguntas para el alumno

1. ¿Cuáles fueron los drivers que más espacio consumían?
2. ¿Se podría automatizar esta reducción de forma incremental con `make localmodconfig`?
3. ¿Qué riesgos puede implicar deshabilitar drivers para el sistema?

---

## :bookmark_tabs: Recursos

- Repositorio oficial del kernel BeagleBone: https://github.com/beagleboard/linux
- Herramienta de configuración: `menuconfig`
- U-Boot Bootloader: https://www.denx.de/wiki/U-Boot

---

