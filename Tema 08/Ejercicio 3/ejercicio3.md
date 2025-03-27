# Crear una Layer Personalizada con un Ejecutable Hello World

## Objetivo

Aprender a crear una meta-layer personalizada en Yocto, incluir en ella un ejecutable en C (hello world) y configurar el entorno para que dicho binario se incluya automáticamente en la imagen `core-image-minimal`.

---

## Parte 1: Crear la layer personalizada

### 1.1 Crear la capa con `bitbake-layers`

Desde el directorio donde tienes `poky`:

```bash
cd poky
source oe-init-build-env
bitbake-layers create-layer ../meta-hello_world
```

Esto generará:

```plaintext
meta-hello_world/
├── conf/
│   └── layer.conf
├── COPYING.MIT
└── README
```

### 1.2 Añadir la nueva capa al entorno de build

```bash
bitbake-layers add-layer ../meta-hello_world
```

Esto actualizará `bblayers.conf` con la ruta a la nueva capa.

**Nota:**También se podría hacer a mano

---

## Parte 2: Añadir la receta y el código fuente

### 2.1 Crear la estructura de directorios

```bash
cd ../meta-hello_world
mkdir -p recipes-apps/hello/files
```

### 2.2 Crear el código fuente `hello.c`

Guarda el siguiente contenido como `recipes-apps/hello/files/hello.c`:

```c
#include <stdio.h>

int main() {
    printf("Hola desde Yocto!\n");
    return 0;
}
```

### 2.3 Crear la receta `hello_1.0.bb`

Archivo: `recipes-apps/hello/hello_1.0.bb`

```bitbake
SUMMARY = "Programa simple Hello World"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING.MIT;md5=0835c914a109f6c7d16c6e8194e2c2e7"

SRC_URI = "file://hello.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${LDFLAGS} hello.c -o hello
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 hello ${D}${bindir}/hello
}
```

> Asegúrate de que `COPYING.MIT` esté disponible o usa el que ya está en `${COREBASE}/meta/files/common-licenses/MIT`.

---

## Parte 3: Incluir el paquete en la imagen final

### 3.1 Añadir el paquete al `local.conf`

Edita `build/conf/local.conf` y añade al final:

```bitbake
IMAGE_INSTALL:append = " hello"
```

> Esto asegura que el binario se incluya en `core-image-minimal`.

---

## Parte 4: Compilar y verificar

### 4.1 Compilar el paquete

```bash
bitbake hello
```

### 4.2 Compilar la imagen con el paquete incluido

```bash
bitbake core-image-minimal
```

### 4.3 Verificar el binario en la imagen

Puedes ejecutar la imagen con QEMU o montar la imagen `.ext4` y comprobar:

```bash
ls /usr/bin/hello
```

o bien iniciar el sistema y ejecutar:

```bash
hello
```

Deberías ver:

```bash
Hola desde Yocto!
```

