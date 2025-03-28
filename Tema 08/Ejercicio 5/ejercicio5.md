# Generación e Instalación del SDK en Yocto

## Objetivo

Aprender a generar un SDK (Software Development Kit) a partir de una imagen en Yocto, instalarlo en el sistema anfitrión y compilar una aplicación para la plataforma embebida desde fuera del entorno Yocto.

---

## Requisitos previos

- Haber generado una imagen previamente, por ejemplo `core-image-minimal`.

---

## Parte 1: Generar el SDK

1. Abre tu entorno de build:

```bash
cd poky
source oe-init-build-env
```

2. Ejecuta el siguiente comando para generar el SDK:

```bash
bitbake core-image-minimal -c populate_sdk
```

3. Al finalizar, el SDK se encontrará en:

```bash
tmp/deploy/sdk/
```

Ejemplo de archivo generado:

```bash
poky-glibc-x86_64-core-image-minimal-armv7at2hf-neon-toolchain-<fecha>.sh
```

---

## Parte 2: Instalar el SDK

1. Ejecuta el script generado:

```bash
./poky-glibc-x86_64-core-image-minimal-<target>.sh
```

2. Sigue las instrucciones. El SDK se instalará (por defecto) en:

```bash
~/poky_sdk/
```

---

## Parte 3: Usar el SDK para compilar una aplicación

1. Carga el entorno de compilación cruzada:

```bash
source ~/poky_sdk/environment-setup-<tripleta-del-target>
```

Ejemplo:

```bash
source ~/poky_sdk/environment-setup-cortexa8t2hf-neon-poky-linux-gnueabi
```

2. Crea un archivo `hello.c`:

```c
#include <stdio.h>

int main() {
    printf("Hola desde el SDK de Yocto!\n");
    return 0;
}
```

3. Compila con el compilador cruzado:

```bash
$CC hello.c -o hello
```

4. Verifica el tipo de binario:

```bash
file hello
```

Debería decir algo como:

```bash
ELF 32-bit LSB executable, ARM, EABI5, ...
```

5. Transfiere el ejecutable al sistema embebido (QEMU o BeagleBone) y ejecútalo:

```bash
./hello
```

Salida esperada:

```bash
Hola desde el SDK de Yocto!
```
