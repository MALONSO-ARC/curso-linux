T# Tema 8: PROYECTO YOCTO

## Introducción al Proyecto Yocto y su Importancia en Embebidos

El **Proyecto Yocto** es una herramienta de desarrollo de sistemas embebidos que permite la creación de distribuciones Linux personalizadas. Su flexibilidad y modularidad lo han convertido en un estándar en la industria embebida, facilitando la creación de sistemas optimizados para hardware específico.

### Beneficios clave:
- Creación de sistemas Linux optimizados y personalizados.
- Uso de capas para gestionar configuraciones y paquetes.
- Reproducibilidad y mantenibilidad del sistema.
- Comunidad activa y soporte de la **Linux Foundation**.

---

## Instalación y Configuración del Entorno de Yocto

### Requisitos del sistema:
- Ubuntu/Debian 64-bit (preferido) o Fedora.
- Instalación de paquetes necesarios:
  ```bash
  sudo apt update && sudo apt install -y gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping
  ```

### Clonación del Proyecto Yocto:
```bash
git clone git://git.yoctoproject.org/poky.git -b dunfell
cd poky
```

### Configuración del Entorno:
```bash
source oe-init-build-env
```
Esto crea un directorio `build` donde se configurará la construcción.

---

## Componentes Principales: Poky, BitBake, Meta-Layers

### **Poky**
Poky es la referencia de implementación en Yocto y contiene:
- **BitBake**: Motor de construcción.
- **Meta-poky**: Capa de referencia con configuraciones base.
- **Meta-openembedded**: Capas adicionales con paquetes complementarios.

### **BitBake**
BitBake es la herramienta encargada de ejecutar las recetas de construcción.

Ejemplo de construcción:
```bash
bitbake core-image-minimal
```

### **Meta-Layers**
Las capas (**meta-layers**) permiten modularizar el sistema. Ejemplo:
- `meta-yocto-bsp`: Contiene BSPs de referencia.
- `meta-openembedded`: Repositorio de paquetes adicionales.

---

## Creación de Imágenes con Yocto

Para generar una imagen básica:
```bash
bitbake core-image-minimal
```
Esto generará una imagen lista para flashear en el hardware.

Para agregar paquetes, edita `conf/local.conf`:
```bash
IMAGE_INSTALL_append = " vim git "
```

---

## Configuración y Personalización de Recetas

Las recetas (`.bb`) definen cómo se construyen los paquetes.
Ejemplo de receta:
```bitbake
SUMMARY = "Mi aplicación personalizada"
LICENSE = "MIT"
SRC_URI = "git://github.com/usuario/miapp.git"
S = "${WORKDIR}/git"
inherit autotools
```

Ejemplo de creación de una capa personalizada:
```bash
bitbake-layers create-layer meta-miapp
```

---

## Gestión de Paquetes en Yocto (`opkg`, `rpm`, `dpkg`)

Yocto soporta varios sistemas de paquetes:
- `opkg`: Ligero, ideal para sistemas embebidos.
- `rpm`: Robusto, usado en distribuciones como Fedora.
- `dpkg`: Base del sistema de paquetes de Debian.

Para generar paquetes en formato `ipk` (usado por `opkg`):
```bash
bitbake mypackage -c package_write_ipk
```

---

## Construcción de un RootFS con Yocto

El **RootFS** es el sistema de archivos base del sistema embebido.

Para generar un `tar.gz` del rootfs:
```bash
bitbake core-image-minimal -c rootfs_tarball
```
Para inspeccionar el contenido:
```bash
tar -tvf tmp/deploy/images/qemux86/core-image-minimal-qemux86.tar.gz
```

---

## Integración de Kernel y U-Boot en Yocto

Para compilar U-Boot:
```bash
bitbake u-boot
```
Para compilar un kernel personalizado:
```bash
bitbake virtual/kernel
```

---

## Creación de un BSP en Yocto

Un **Board Support Package (BSP)** incluye:
- Soporte para el kernel.
- Drivers necesarios.
- Configuraciones personalizadas.

Estructura de un BSP:
```
meta-mi-bsp/
├── conf/
│   ├── layer.conf
├── recipes-bsp/
│   ├── u-boot/
│   ├── linux/
```

Para agregar un BSP personalizado:
```bash
bitbake-layers add-layer meta-mi-bsp
```

---

## Debugging y Optimización de Yocto

Herramientas de debugging:
- `bitbake -e`: Ver variables de entorno.
- `bitbake -c devshell <paquete>`: Entrar en shell de desarrollo.
- `bitbake -c cleansstate <paquete>`: Limpiar y reconstruir.

Para optimizar el tiempo de construcción:
- Habilitar compilación paralela:
  ```bash
  BB_NUMBER_THREADS = "8"
  PARALLEL_MAKE = "-j8"
  ```
- Usar `sstate-cache` para reducir tiempos de recompilación.

---

## ¿Cómo detectar qué necesitamos y reducir el tamaño necesario?

- **Eliminar paquetes innecesarios:**
  ```bash
  IMAGE_FEATURES_remove = "package-management"
  ```
- **Reducir el núcleo a lo esencial:**
  ```bash
  bitbake linux-yocto -c menuconfig
  ```
- **Usar BusyBox en lugar de GNU Coreutils.**
- **Eliminar debug symbols:**
  ```bash
  INHERIT_remove = "rm_work"
  ```

