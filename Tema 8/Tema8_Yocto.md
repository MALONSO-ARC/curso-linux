# Tema 8: Proyecto Yocto

## 8.1 Introducción al Proyecto Yocto y su Importancia en Embebidos
El **Proyecto Yocto** es una plataforma de código abierto que permite la creación de sistemas operativos personalizados para dispositivos embebidos. Su flexibilidad y modularidad lo han convertido en una de las herramientas más utilizadas en la industria.

### Beneficios de Yocto en sistemas embebidos:
- Generación de sistemas optimizados y personalizados.
- Mantenimiento de un entorno reproducible y escalable.
- Gran comunidad y soporte de la industria.

## 8.2 Instalación y Configuración del Entorno de Yocto
Antes de empezar a trabajar con Yocto, es necesario instalar las dependencias y preparar el entorno de desarrollo.

### Requisitos previos:
- Distribución Linux (Ubuntu/Debian/Fedora).
- Paquetes necesarios: `git`, `tar`, `wget`, `gcc`, `g++`, `make`.

### Pasos de instalación:
1. Clonar el repositorio de Poky:

   ```bash
   git clone git://git.yoctoproject.org/poky.git
   cd poky
   ```
2. Configurar el entorno:

   ```bash
   source oe-init-build-env
   ```

## 8.3 Componentes Principales: Poky, BitBake, Meta-layers
- **Poky**: Referencia base del sistema Yocto.

- **BitBake**: Herramienta principal para la construcción de paquetes.

- **Meta-layers**: Capas de configuración que permiten extender las funcionalidades de Yocto.


## 8.4 Creación de Imágenes con Yocto
Para generar una imagen con Yocto:

```bash
bitbake core-image-minimal
```
Esto generará un sistema básico optimizado para embebidos.

## 8.5 Configuración y Personalización de Recetas
Las recetas en Yocto definen cómo construir paquetes y sistemas completos. Se pueden personalizar agregando configuraciones específicas en `meta-layers`.

Ejemplo de receta mínima:

```bash
DESCRIPTION = "Ejemplo de receta personalizada"
LICENSE = "MIT"
SRC_URI = "file://archivo.tar.gz"
S = "${WORKDIR}"
do_compile() {
    make
}
do_install() {
    install -D -m 0755 binario ${D}${bindir}/binario
}
```

## 8.6 Gestión de Paquetes en Yocto (`opkg`, `rpm`, `dpkg`)
Yocto soporta varios sistemas de gestión de paquetes:

- **opkg**: Ligero y eficiente, usado en OpenEmbedded.

- **rpm**: Potente y escalable, usado en distribuciones como Fedora.

- **dpkg**: Compatible con Debian y sus derivados.


Ejemplo de generación de paquetes:

```bash
bitbake packagegroup-core-base
```

## 8.7 Construcción de un RootFS con Yocto
Para generar un sistema de archivos raíz (RootFS):

```bash
bitbake core-image-full-cmdline
```
Esto generará una imagen con herramientas adicionales en `/rootfs`.

## 8.8 Integración de Kernel y U-Boot en Yocto
Se pueden integrar el Kernel y el bootloader (U-Boot) en Yocto mediante `meta-layers` personalizados.

Ejemplo de compilación del Kernel:

```bash
bitbake virtual/kernel
```

## 8.9 Creación de un BSP en Yocto
Un **BSP (Board Support Package)** permite la compatibilidad con hardware específico. Se define en `meta-layers` y se configura mediante los archivos `machine.conf`.

## 8.10 Debugging y Optimización de Yocto
Para depurar y optimizar, se pueden usar herramientas como:

- **bitbake -g target** (ver dependencias de paquetes)

- **devshell** (entorno de desarrollo para debugging)

- **Yocto Autobuilder** (automatización de pruebas y compilaciones)


## 8.11 ¿Cómo Detectar Qué Necesitamos y Reducir el Tamaño Necesario?
Para reducir el tamaño de la imagen final:

- **Eliminar paquetes innecesarios** en `local.conf`.

- **Usar SquashFS o compresión gzip/lz4**.

- **Minimizar logs y herramientas de depuración**.


Ejemplo de reducción de tamaño:

```bash
EXTRA_IMAGE_FEATURES = "debug-tweaks"
IMAGE_FEATURES_remove = "package-management"
```

## Conclusión
El Proyecto Yocto es una herramienta clave en el desarrollo de sistemas embebidos. Su modularidad, flexibilidad y enfoque en la personalización lo hacen una opción poderosa para crear soluciones optimizadas y eficientes.
