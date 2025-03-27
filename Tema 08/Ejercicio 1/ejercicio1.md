# Configuración Inicial del Proyecto Yocto para BeagleBone

## Objetivo

Aprender a descargar el código fuente del proyecto Yocto (Poky) y configurar el entorno de compilación para generar una imagen mínima para la plataforma BeagleBone Black.



## Parte 1: Descargar Poky y sus capas

1. Clona el repositorio oficial de Poky:

```bash
git clone -b scarthgap https://github.com/yoctoproject/poky.git
cd poky
```

2. (Opcional) Clona capas adicionales:

```bash
git clone https://git.openembedded.org/meta-openembedded
```

---

## Parte 2: Inicializar el entorno de construcción

1. Ejecuta el script de entorno:

```bash
source oe-init-build-env
```

Esto creará y entrará al directorio `build/`.

---

## Parte 3: Configurar la plataforma objetivo (BeagleBone)

1. Edita el archivo `conf/local.conf`:

```bash
nano conf/local.conf
```

Modifica las siguientes líneas:

```conf
MACHINE = "beaglebone"
```

2. Edita el archivo `conf/bblayers.conf` para incluir las capas necesarias:

```bash
nano conf/bblayers.conf
```

Asegúrete de tener rutas como:

```conf
BBLAYERS ?= " \
  ${TOPDIR}/../poky/meta \
  ${TOPDIR}/../poky/meta-poky \
  ${TOPDIR}/../meta-ti \
"
```

> Añade también `meta-openembedded/meta-oe` si vas a usar paquetes adicionales.

---

## Parte 4: Compilar una imagen mínima

1. Ejecuta el siguiente comando:

```bash
bitbake core-image-minimal
```

Esto puede tardar bastante tiempo la primera vez (dependiendo de tu CPU/RAM/conexión).

---

## Parte 5: Verificar los resultados

1. Cuando termine, ve a:

```bash
ls tmp/deploy/images/beaglebone/
```

Deberías ver archivos como:

- `core-image-minimal-beaglebone.ext4`
- `core-image-minimal-beaglebone.tar.xz`
- `u-boot.img`, `MLO` (bootloader)
- `zImage` o `Image` (kernel)
- `device tree` (`.dtb`)

---

## Preguntas de Reflexión

1. ¿Qué diferencia hay entre una imagen tipo `.ext4` y `.tar.xz` en Yocto?
2. ¿Qué rol cumple la capa `meta-ti`?
3. ¿Por qué es importante usar la variable `MACHINE` correctamente?

---

