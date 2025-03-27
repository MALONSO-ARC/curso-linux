# Tema 7: Sistema de Archivos Root (RootFS)

## 7.1 ¿Qué es RootFS y por qué es importante?
El RootFS (Root File System) es el sistema de archivos raíz en un sistema Linux embebido. Contiene todos los archivos y directorios esenciales necesarios para el arranque y la operación del sistema. Su correcto diseño y configuración afectan directamente el rendimiento, la seguridad y la capacidad de actualización del sistema embebido.

## 7.2 Tipos de Sistemas de Archivos en Linux Embebido
Existen varios tipos de sistemas de archivos utilizados en Linux embebido, cada uno con características específicas:
- **ext4**: Común en sistemas de almacenamiento estándar, soporta journaling.
- **JFFS2**: Diseñado para memorias flash NOR, adecuado para sistemas con escrituras frecuentes.
- **UBIFS**: Optimizado para memorias flash NAND, ofrece mejor rendimiento y compresión.
- **SquashFS**: Sistema de archivos comprimido de solo lectura, ideal para sistemas con almacenamiento limitado.

## 7.3 Uso de Initramfs y RootFS Personalizados
- **Initramfs**: Sistema de archivos inicial cargado en RAM que facilita el arranque.
- **RootFS Personalizado**: Adaptado a las necesidades del sistema embebido, minimizando tamaño y optimizando rendimiento.

## 7.4 Estructura de Directorios en Linux Embebido
[Linux Filesystem Hierarchy](https://refspecs.linuxfoundation.org/FHS_3.0/fhs/index.html)
Los sistemas Linux embebidos siguen una estructura de directorios similar a las distribuciones estándar, pero optimizada para entornos de recursos limitados:
- `/bin`: Binarios esenciales.
- `/lib`: Bibliotecas compartidas.
- `/dev`: Archivos de dispositivos.
- `/sys`: Información sobre el sistema y los dispositivos.

## 7.5 Creación de RootFS con BusyBox

BusyBox es una colección de utilidades UNIX optimizadas para sistemas embebidos. Proporciona comandos como `ls`, `cp`, `sh`, etc., en una sola aplicación compacta, ideal para entornos con recursos limitados.

A continuación se detallan los pasos para crear un sistema de archivos root (rootfs) básico utilizando BusyBox.

---

### 1. Configuración y compilación de BusyBox

#### a) Descargar BusyBox
```bash
wget https://busybox.net/downloads/busybox-1.36.1.tar.bz2
```

#### b) Descomprimir el paquete
```bash
tar xjf busybox-1.36.1.tar.bz2
cd busybox-1.36.1
```

#### c) Configurar opciones por defecto
```bash
make defconfig
```

#### d) (Opcional) Personalizar con `menuconfig`
```bash
make menuconfig
# Opcional: establecer "BusyBox installation prefix" como ../rootfs
```

#### e) Compilar
```bash
make -j$(nproc)
```

---

### 2. Instalación en un directorio base

#### a) Crear directorio para el rootfs
```bash
mkdir -p ../rootfs
```

#### b) Instalar los binarios de BusyBox
```bash
make CONFIG_PREFIX=../rootfs install
```

Esto instalará en `../rootfs` los binarios de BusyBox y enlaces simbólicos a comandos como `ls`, `cat`, `sh`, etc.

---

### 3. Creación de los directorios esenciales

Un sistema de archivos funcional requiere ciertas carpetas base (Ver [LFH](https://refspecs.linuxfoundation.org/FHS_3.0/fhs/index.html)):
```bash
mkdir -p ../rootfs/{dev,proc,sys,tmp,etc,lib,usr,home,mnt}
```

#### a) Crear nodos de dispositivos mínimos:
```bash
sudo mknod -m 622 ../rootfs/dev/console c 5 1
sudo mknod -m 666 ../rootfs/dev/null c 1 3
```

#### b) Crear script de arranque `/init` (si no se usa BusyBox `init`)
```bash
cat << 'EOF' > ../rootfs/init
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
echo "Sistema inicializado"
exec /bin/sh
EOF
chmod +x ../rootfs/init
```

---

### 4. Generación de imágenes del sistema de archivos

#### a) Crear initramfs (`cpio.gz`)
```bash
cd ../rootfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
```

#### b) (Opcional) Crear `uInitrd` para U-Boot
```bash
mkimage -A arm -T ramdisk -C gzip -n "Initramfs" \
  -d ../initramfs.cpio.gz ../uInitrd
```

---

### Ejemplo de uso con QEMU (vexpress-a9)

```bash
qemu-system-arm -M vexpress-a9 -m 512M \
  -kernel zImage \
  -dtb vexpress-v2p-ca9.dtb \
  -initrd initramfs.cpio.gz \
  -append "console=ttyAMA0" \
  -nographic
```

---

### Notas

- El script `/init` se ejecuta si no hay `init` en `/sbin` o `/etc`.
- BusyBox puede configurarse para actuar como `init` directamente.
- Se puede añadir soporte a más comandos en `menuconfig` (por ejemplo: `vi`, `telnetd`, etc.)


## 7.6 Métodos de Carga del RootFS en Sistemas Embebidos
---

# Tema 7 - Sistemas de Archivos Root (RootFS)

## 7.6 Métodos de Carga del RootFS en Sistemas Embebidos

En sistemas embebidos, el sistema de archivos root (rootfs) puede cargarse de diversas formas, dependiendo del hardware, el gestor de arranque y las necesidades del sistema.

A continuación se detallan los principales métodos utilizados:

### 1. Montaje desde Memoria Flash (eMMC, NAND, NOR)

El rootfs se encuentra almacenado en una partición del dispositivo de almacenamiento interno no volátil del sistema.

#### Ventajas:
- Persistente.
- Rápido acceso.
- Común en productos finales.

#### Requisitos:
- U-Boot debe conocer la ubicación del kernel y el rootfs.
- El kernel debe tener soporte para el sistema de archivos (ej: ext4, ubifs).

#### Ejemplo de bootargs:
```bash
setenv bootargs console=ttyO0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait
```

---

### 2. Uso de Initramfs en RAM

El rootfs se empaqueta como un archivo `initramfs.cpio.gz` (o `uInitrd`) y se carga en RAM junto al kernel.

#### Ventajas:
- Muy rápido.
- Ideal para sistemas sin almacenamiento persistente.
- Fácil de probar y depurar.

#### Requisitos:
- Debe cargarse en memoria desde U-Boot o estar embebido en el kernel.
- Tamaño limitado por la RAM disponible.

#### Ejemplo de bootargs:
```bash
setenv bootargs console=ttyO0,115200 root=/dev/ram rw
```

---

### 3. Carga desde Red usando NFS

El rootfs se encuentra en el host y se exporta vía NFS. El sistema embebido lo monta vía red.

#### Ventajas:
- Muy útil para desarrollo: edición en vivo.
- No requiere escritura en memoria flash.

#### Requisitos:
- Red funcional (configurada por U-Boot o kernel).
- Servidor NFS exportando el rootfs.

#### Ejemplo de bootargs:
```bash
setenv bootargs console=ttyO0,115200 root=/dev/nfs nfsroot=192.168.1.100:/home/user/rootfs ip=dhcp rw
```

---

### 4. Montaje desde Almacenamiento Externo (USB, SD)

El rootfs está en un dispositivo extraíble como una memoria USB o tarjeta SD.

#### Ventajas:
- Fácil de intercambiar entre versiones.
- Ideal para mantenimiento, pruebas o recuperación.

#### Requisitos:
- El bootloader debe soportar el medio (ej: `usb start`, `mmc rescan`).
- El kernel debe tener soporte para el sistema de archivos correspondiente.

#### Ejemplo de bootargs:
```bash
setenv bootargs console=ttyO0,115200 root=/dev/sda1 rootfstype=ext4 rootwait
```

## 7.7 Configuración de Permisos y Seguridad en RootFS

La configuración adecuada de permisos en el rootfs es fundamental para proteger el sistema embebido contra accesos no autorizados, manipulación de archivos críticos o ejecución de código malicioso.

### 1. Definición de permisos adecuados para archivos y directorios

El sistema de archivos debe seguir buenas prácticas de permisos:

- Los directorios deben tener al menos permisos `x` para ser accesibles.
- Los archivos ejecutables deben tener permiso de ejecución (`+x`).
- Archivos de configuración sensibles deben tener permisos restringidos (`600`, `644`).

Ejemplos comunes:
```bash
chmod 755 /bin/busybox
chmod 644 /etc/inittab
chmod 600 /etc/shadow
```

### 2. Uso de herramientas como `chown` y `chmod`

Para ajustar propietarios y permisos dentro del rootfs:

```bash
chown root:root /sbin/init
chmod 755 /sbin/init
```

También es importante asegurarse de que scripts personalizados no se dejen como ejecutables si no es necesario:
```bash
chmod -x /etc/banner.sh
```

Estos comandos pueden ejecutarse manualmente o automatizarse desde un script post-instalación.

### 3. Aplicación de políticas de seguridad como SELinux o AppArmor

En sistemas más complejos, se pueden aplicar políticas de seguridad a nivel de sistema:

#### SELinux (Security-Enhanced Linux):
- Basado en etiquetas.
- Controla el acceso a recursos según contexto de seguridad.
- Se configura con reglas y políticas.
- Útil en dispositivos con requerimientos estrictos de seguridad.

#### AppArmor:
- Basado en perfiles por binario.
- Más fácil de configurar que SELinux.
- Limita el acceso de binarios a rutas específicas.

> Nota: Para sistemas embebidos ligeros, SELinux y AppArmor no siempre están disponibles o habilitados por defecto. Su uso depende del kernel, la distribución base (ej: Poky/Yocto) y las capacidades del sistema.

## 7.8 Soporte para Montaje Remoto con NFS

NFS (Network File System) permite que un sistema embebido acceda a un sistema de archivos remoto exportado por un servidor a través de la red. Este método es especialmente útil durante el desarrollo, ya que permite modificar archivos del sistema de manera remota sin tener que reescribir la memoria del dispositivo.

A continuación se detallan los pasos básicos para configurar y utilizar NFS en un entorno de desarrollo embebido.

---

### 1. Configuración del servidor NFS (en el host Linux)

#### a) Instalar los paquetes necesarios
```bash
sudo apt update
sudo apt install nfs-kernel-server
```

#### b) Crear el directorio a exportar
```bash
sudo mkdir -p /export/rootfs
sudo chown -R nobody:nogroup /export/rootfs
```

#### c) Editar `/etc/exports` para definir el acceso
```bash
sudo nano /etc/exports
```
Y añadir la siguiente línea:
```
/export/rootfs  *(rw,sync,no_subtree_check,no_root_squash)
```

#### d) Aplicar la configuración
```bash
sudo exportfs -a
sudo systemctl restart nfs-kernel-server
```

#### e) Verificar que el directorio se exporta
```bash
showmount -e
```

---

### 2. Configuración del cliente NFS (sistema embebido)

#### a) Configurar el bootargs en U-Boot
```bash
setenv bootargs console=ttyO0,115200 root=/dev/nfs nfsroot=192.168.1.100:/export/rootfs,v3,tcp ip=dhcp rw
```
> Reemplaza `192.168.1.100` con la IP de tu host

#### b) Asegurarse de que el kernel tenga soporte para NFS
- Configurado como `y` o `m` en `make menuconfig`:
  - `File Systems → Network File Systems → NFS client support`

---

### 3. Montaje del sistema de archivos remoto

Al arrancar, el kernel del sistema embebido:
- Obtendrá la dirección IP por DHCP.
- Montará `/export/rootfs` como raíz (`/`).
- Iniciará desde allí sin necesidad de memoria flash local.

#### Ejemplo con QEMU:
```bash
qemu-system-arm -M vexpress-a9 -m 512M \
  -kernel zImage -dtb vexpress-v2p-ca9.dtb \
  -append "console=ttyAMA0 root=/dev/nfs nfsroot=10.0.2.2:/export/rootfs,v3,tcp ip=dhcp rw" \
  -nographic
```

---

### Ventajas de NFS
- Ideal para desarrollo: cambios reflejados de inmediato.
- No requiere escribir en la memoria del sistema embebido.
- Permite trabajo colaborativo en un entorno compartido.

### Consideraciones
- Necesita red funcional y estable.
- No se recomienda en entornos de producción.

## 7.9 Compresión y Optimización del RootFS

En sistemas embebidos, el espacio de almacenamiento es limitado. Por ello, es fundamental aplicar técnicas de compresión y optimización al sistema de archivos root (rootfs) para reducir su tamaño y mejorar su rendimiento.

---

### 1. Uso de SquashFS para un RootFS comprimido de solo lectura

**SquashFS** es un sistema de archivos comprimido y de solo lectura, ideal para sistemas embebidos que no requieren escritura sobre el rootfs.

#### Ventajas:
- Alta compresión.
- Montaje rápido desde almacenamiento flash.
- Protege la integridad del sistema base (no modificable).

#### Requisitos:
- Soporte para SquashFS en el kernel (`CONFIG_SQUASHFS=y`).
- Herramienta `mksquashfs` instalada en el host.

#### Ejemplo de generación:
```bash
mksquashfs rootfs/ rootfs.squashfs -comp xz -e boot
```
> `-comp xz` especifica el algoritmo de compresión (tambén se puede usar `zstd`, `gzip`, `lz4`).

---

### 2. Eliminación de archivos innecesarios

Antes de generar el rootfs definitivo, es buena práctica:

- Eliminar archivos de desarrollo: headers, ejemplos, docs.
- Quitar comandos no utilizados (en BusyBox o paquetes).
- Usar `strip` en binarios para eliminar símbolos de depuración:

```bash
arm-linux-gnueabihf-strip rootfs/bin/*
```

- Verificar espacio ocupado:
```bash
du -sh rootfs/
```

---

### 3. Compresión avanzada con `zstd`

`zstd` es un algoritmo moderno que ofrece alta compresión y velocidad. Puedes usarlo en combinación con `mksquashfs` si el kernel lo soporta.

#### Generar squashfs con zstd:
```bash
mksquashfs rootfs/ rootfs.squashfs -comp zstd
```

#### Verificar soporte en el kernel:
```bash
zcat /proc/config.gz | grep SQUASHFS_ZSTD
```

---

### 4. Montaje del SquashFS en el sistema embebido

En el `bootargs` de U-Boot:
```bash
setenv bootargs console=ttyO0,115200 root=/dev/loop0 rootfstype=squashfs ro
```
Y cargar el archivo `rootfs.squashfs` desde almacenamiento o initramfs.


## 7.10 Debugging de Problemas en RootFS

Cuando un sistema embebido presenta fallos en el arranque, errores al acceder a archivos o comportamientos anómalos relacionados con el sistema de archivos raíz (RootFS), es importante aplicar técnicas de depuración sistemáticas para identificar y resolver el problema.

A continuación se describen herramientas y métodos útiles para diagnosticar fallos en el RootFS.

---

### 1. Análisis de logs del sistema

Los mensajes del sistema son una fuente fundamental para detectar errores en el montaje del rootfs, falta de permisos, módulos faltantes, etc.

#### Herramientas principales:

- `dmesg`: muestra los mensajes del kernel desde el arranque.
```bash
dmesg | less
```

- `/var/log/messages` o `/var/log/syslog`: contienen logs del sistema y servicios.
```bash
cat /var/log/messages
journalctl -xe
```

#### Qué buscar:
- Fallos en el montaje de sistemas de archivos.
- Problemas con controladores de almacenamiento.
- Errores de permisos en archivos críticos.

---

### 2. Uso de `strace` y `lsof` para monitoreo de acceso a archivos

Estas herramientas permiten rastrear las llamadas al sistema y detectar qué archivos se están accediendo (o fallando en acceder).

#### `strace`:
Permite trazar llamadas del sistema de un proceso.
```bash
strace -o salida.txt /bin/busybox sh
```

Revisar el archivo `salida.txt` para ver qué archivos intenta abrir o montar.

#### `lsof`:
Muestra los archivos abiertos por procesos en ejecución.
```bash
lsof | grep /etc
```

Muy útil para detectar archivos bloqueados o con permisos incorrectos.

---

### 3. Verificación de integridad con `fsck` y `debugfs`

Si se sospecha de corrupción del sistema de archivos, estas herramientas permiten verificarlo y repararlo.

#### `fsck`:
Verifica y repara sistemas de archivos (solo para rootfs persistente como ext4).
```bash
fsck.ext4 /dev/mmcblk0p2
```

> ⚠️ Debe ejecutarse con el sistema desmontado o en modo recuperación.

#### `debugfs`:
Permite inspeccionar el sistema de archivos a bajo nivel.
```bash
debugfs /dev/mmcblk0p2
```

Ejemplo de comandos dentro de `debugfs`:
```bash
ls /
stat /sbin/init
```

---

### Consejos adicionales

- Asegúrate de que `/sbin/init` o `/init` exista y tenga permisos de ejecución.
- Verifica que todos los dispositivos esenciales estén presentes en `/dev`.
- Compara con una versión funcional del rootfs si tienes una copia de respaldo.




