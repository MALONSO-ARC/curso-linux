---

# Crear una Imagen Personalizada en Yocto

## Objetivo

Aprender a crear una imagen customizada en Yocto que incluya paquetes definidos por el usuario y características específicas para el sistema embebido.

---

## 1. Crear el directorio de la imagen personalizada

Dentro de tu capa personalizada (por ejemplo, `meta-miempresa`):

```bash
mkdir -p meta-miempresa/recipes-core/images
```

---

## 2. Crear la receta de imagen: `core-image-miempresa.bb`

Archivo: `meta-miempresa/recipes-core/images/core-image-miempresa.bb`

```bitbake
DESCRIPTION = "Imagen personalizada con utilidades básicas y hello"
LICENSE = "MIT"

inherit core-image

IMAGE_FEATURES += "ssh-server-dropbear"

IMAGE_INSTALL += "hello htop nano"
```

> Usa `inherit core-image` para heredar el comportamiento de `core-image` base.

---

## 3. Añadir la capa personalizada al entorno Yocto

Asegúrate de que la capa `meta-miempresa` esté incluida en `build/conf/bblayers.conf`.

Si no lo está, añade:

```conf
BBLAYERS += "${TOPDIR}/../meta-miempresa"
```

---

## 4. Compilar la imagen personalizada

Desde tu entorno de build:

```bash
bitbake core-image-miempresa
```

---

## 5. Verificar los resultados

Después de la compilación, la imagen estará disponible en:

```bash
tmp/deploy/images/<machine>/core-image-miempresa-<machine>.ext4
```

Puedes montar el `.ext4` o lanzarla en QEMU para verificar los binarios.

---

## Variables útiles para personalización

| Variable                    | Descripción                                                             |
|-----------------------------|-------------------------------------------------------------------------|
| `IMAGE_INSTALL`            | Lista de paquetes que se instalarán en el rootfs                       |
| `IMAGE_FEATURES`           | Características habilitadas (debug, ssh, tools, etc.)                  |
| `IMAGE_FSTYPES`            | Tipos de imagen generada (`ext4`, `wic`, `tar.gz`, etc.)               |
| `ROOTFS_POSTPROCESS_COMMAND` | Comandos adicionales para ejecutar tras generar el rootfs            |

---

## Ejemplo completo: `core-image-desarrollo.bb`

```bitbake
DESCRIPTION = "Imagen de desarrollo embebido"
LICENSE = "MIT"

inherit core-image

IMAGE_FEATURES += "debug-tweaks ssh-server-dropbear tools-debug"

IMAGE_INSTALL += "hello htop nano i2c-tools strace"
```

---

## Verificar contenido del rootfs

Montar el sistema de archivos generado. Para ello ejecuta el comando:

```bash
runqemu
```

---

