# Tema 4: Toolchain (Herramientas de Compilación)

## Introducción al concepto de toolchain
Una **toolchain** es un conjunto de herramientas utilizadas para compilar y construir software, incluyendo:

- **Compilador** (GCC, Clang)
- **Enlazador** (ld)
- **Bibliotecas estándar** (glibc, musl, uClibc)
- **Depuradores** (GDB)

## Diferencias entre toolchain nativa y cruzada
- **Nativa**: Se ejecuta en la misma arquitectura que el software que compila.
- **Cruzada**: Se ejecuta en una arquitectura diferente a la del software objetivo.

## Componentes de una toolchain
- **Compilador (GCC, Clang)**
- **Binutils (as, ld, objdump, etc.)**
- **Bibliotecas estándar (glibc, musl, uClibc)**

## Introducción a GNU Toolchain
El ecosistema GNU Toolchain incluye:

- **GCC**: Compilador de código.
- **Binutils**: Herramientas de ensamblado y manipulación de binarios.
- **GLIBC**: Biblioteca estándar de C.

## Configuración y compilación cruzada con GCC
Ejemplo de compilación cruzada:
```bash
arm-linux-gnueabihf-gcc -o hola hola.c
```

## Uso de toolchains preconstruidas vs. compilación personalizada
- **Preconstruidas**: Rápidas y fáciles de usar.
- **Compiladas a medida**: Más optimizadas para el hardware.

## Diferencias entre enlaces estáticos y dinámicos
- **Estático**: Todo el código se incorpora en el binario.
- **Dinámico**: Usa bibliotecas compartidas externas.

## Creación de una toolchain con Buildroot y Yocto
Buildroot y Yocto permiten generar toolchains personalizadas.

Ejemplo de uso en Buildroot:
```bash
make menuconfig
make
```

---
**Fin del Tema 4**
