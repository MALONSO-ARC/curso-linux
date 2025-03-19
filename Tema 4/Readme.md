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

## Introducción al concepto de toolchain

### ¿Qué es una toolchain?

Una **toolchain** es un conjunto de herramientas utilizadas para compilar, enlazar y depurar software. En el contexto de sistemas embebidos, la toolchain es fundamental para traducir código fuente en ejecutables que puedan correr en la plataforma objetivo.

La **toolchain cruzada** (*cross toolchain*) es el tipo más común en desarrollo embebido, ya que permite compilar código en un sistema anfitrión (*host*), generalmente una PC con Linux, y generar ejecutables para una arquitectura diferente en el sistema embebido (*target*).

Los componentes principales de una toolchain son:

### Compilador

El compilador es el programa encargado de convertir el código fuente en código de máquina para la arquitectura objetivo. Algunas opciones comunes son:

- **GCC (GNU Compiler Collection):** Es el compilador más usado en sistemas Linux embebidos. Soporta múltiples arquitecturas y ofrece optimizaciones avanzadas.
- **Clang:** Basado en LLVM, es una alternativa a GCC que se está volviendo popular por su velocidad y capacidad de análisis estático.

Ejemplo de uso de GCC para compilación cruzada:

```bash
arm-linux-gnueabi-gcc -o mi_programa mi_programa.c
```

### Enlazador (Linker)

El enlazador es el responsable de combinar múltiples archivos objeto en un solo ejecutable. También resuelve referencias a funciones y variables externas.

- En toolchains de GNU, el enlazador es **`ld`**.
- Especificar bibliotecas en el proceso de enlace:

```bash
arm-linux-gnueabi-gcc -o mi_programa mi_programa.o -lm
```

(Aquí `-lm` indica que se enlazará con la biblioteca matemática `libm`.)

### Bibliotecas estándar

Las bibliotecas estándar proporcionan funciones esenciales para los programas en C y C++. En sistemas embebidos, la elección de la biblioteca afecta tanto el rendimiento como el tamaño del ejecutable:

- **glibc (GNU C Library):** La biblioteca estándar de GNU. Rica en características pero puede ser pesada para dispositivos con recursos limitados.
- **musl:** Ligera y eficiente, es una opción popular para sistemas embebidos.
- **uClibc:** Diseñada para entornos embebidos, pero con menor soporte que `glibc` y `musl`.

Ver qué bibliotecas usa un binario:

```bash
ldd mi_programa
```

### Depuradores

Los depuradores permiten analizar y corregir errores en programas compilados para sistemas embebidos:

- **GDB (GNU Debugger):** El depurador más usado en Linux.
- **gdbserver:** Permite depuración remota en dispositivos embebidos.

Ejemplo de depuración remota con GDB:

```bash
gdbserver :1234 mi_programa
```

En el host:

```bash
arm-linux-gnueabi-gdb mi_programa
(gdb) target remote 192.168.1.100:1234
```

### Otras herramientas útiles en una toolchain

- **`objdump`** – Analiza el contenido de un ejecutable.
- **`readelf`** – Muestra información detallada de archivos ELF.
- **`strip`** – Reduce el tamaño de los binarios eliminando símbolos de depuración.
- **`addr2line`** – Traduce direcciones de memoria en archivos fuente y líneas de código.


## Configuración y compilación cruzada con GCC
Ejemplo de compilación cruzada:
```bash
arm-linux-gnueabihf-gcc -o hola hola.c
```

## Uso de toolchains preconstruidas vs. compilación personalizada

En un sistema Linux Embebido, la toolchain es fundamental para compilar todos los componentes del sistema. Existen diferentes formas de obtener una toolchain en el entorno de desarrollo (host), cada una con ventajas y desventajas. A continuación, se explican las opciones más comunes.

### Paquetes Precompilados

Algunas distribuciones de Linux incluyen toolchains precompiladas que pueden instalarse fácilmente mediante su gestor de paquetes:

- **Ubuntu/Debian:**
  ```bash
  sudo apt install gcc-arm-linux-gnueabi
  sudo apt install gcc-aarch64-linux-gnu
  ```
- **Fedora:**
  ```bash
  sudo dnf install arm-none-eabi-gcc
  ```
- **Arch Linux:**
  ```bash
  sudo pacman -S arm-none-eabi-gcc
  ```

#### Ventajas:
- Fácil instalación y mantenimiento.
- Adecuado para desarrollo rápido y pruebas.

#### Desventajas:
- Puede no estar optimizado para el hardware objetivo.
- Puede carecer de algunas bibliotecas necesarias.

### Toolchains Precompiladas de Terceros (Linaro, Sourcery CodeBench, ARM)

Existen toolchains optimizadas mantenidas por comunidades especializadas y fabricantes de hardware:

- **Linaro Toolchain:** Optimizada para procesadores ARM.
  - Descarga en: [Linaro](https://www.linaro.org/downloads/)
- **Sourcery CodeBench (Mentor Graphics)**
- **ARM GNU Toolchain:**
  - Descarga en: [ARM Developer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain)

#### Ventajas:
- Optimización específica para ARM.
- Mantenidas por expertos en la arquitectura.

#### Desventajas:
- Limitada personalización.
- Puede ser necesario adaptarlas para necesidades específicas.

### Compilación Manual de una Toolchain

Para obtener una toolchain completamente personalizada, se puede compilar GCC y sus componentes manualmente. Este proceso implica:

1. Descargar fuentes de **GCC**, **Binutils** y **Glibc**.
2. Compilar Binutils.
3. Compilar GCC (fase 1).
4. Compilar Glibc.
5. Compilar GCC (fase 2).

Para simplificar este proceso, se puede utilizar **crosstool-NG**:

```bash
git clone https://github.com/crosstool-ng/crosstool-ng.git
cd crosstool-ng
./bootstrap && ./configure --enable-local && make && make install
ct-ng menuconfig  # Configurar la toolchain
ct-ng build
```

#### Ventajas:
- Total personalización.
- Optimización para hardware específico.

#### Desventajas:
- Proceso complejo y propenso a errores.
- Toma tiempo compilarla.


### **Conclusión**

| Opción | Facilidad de uso | Personalización | Optimización |
|--------|----------------|----------------|-------------|
| Paquetes precompilados | Alta | Baja | Media |
| Toolchains precompiladas (Linaro, Sourcery) | Media | Baja | Alta |
| Compilación manual | Baja | Alta | Alta |
| Generación con Buildroot/Yocto | Media | Alta | Alta |

Para proyectos simples, los paquetes precompilados o Linaro pueden ser suficientes. Para dispositivos embebidos con requerimientos específicos, compilar la toolchain manualmente o usar Buildroot/Yocto es la mejor opción.


## Diferencias entre enlaces estáticos y dinámicos

Cuando se compila un programa en un sistema Linux embebido, se puede optar por dos tipos de enlaces para las bibliotecas requeridas:

- **Enlace estático:** Las bibliotecas se incluyen completamente dentro del ejecutable final. Esto produce archivos binarios más grandes, pero con la ventaja de que no dependen de bibliotecas externas en el sistema destino. Se usa en entornos donde la disponibilidad de bibliotecas dinámicas es incierta.
- **Enlace dinámico:** El ejecutable referencia bibliotecas compartidas en el sistema destino. Los binarios son más pequeños y permiten reutilizar código común en múltiples aplicaciones, pero requieren que las bibliotecas estén disponibles en el entorno de ejecución.

## Bibliotecas estándar: glibc, musl, uClibc

Las bibliotecas C estándar proporcionan funciones esenciales para el desarrollo en C en sistemas Linux embebidos. Las más usadas son:

- **glibc (GNU C Library):** La opción más completa y estándar en sistemas Linux. Ofrece compatibilidad con POSIX y soporte avanzado, pero su tamaño y consumo de recursos pueden ser excesivos para sistemas embebidos.
- **musl:** Diseñada para ser ligera y eficiente. Mantiene compatibilidad con POSIX y es una opción viable para sistemas con restricciones de memoria.
- **uClibc:** Optimizada para sistemas embebidos con requisitos mínimos de almacenamiento y memoria. A diferencia de musl, permite una mayor personalización en la compilación.

## Alternativas a GNU Toolchain (LLVM, Clang)

Aunque la toolchain GNU (GCC, binutils, glibc) es la más utilizada en entornos embebidos, existen alternativas como:

- **LLVM/Clang:** Un compilador modular con mejor optimización en ciertos casos y tiempos de compilación más rápidos. Es compatible con múltiples arquitecturas y está ganando popularidad en sistemas embebidos.
- **Linaro Toolchain:** Basada en GCC pero optimizada para arquitecturas ARM, usada frecuentemente en sistemas embebidos con procesadores ARM.
- **Picolibc:** Una biblioteca C diseñada específicamente para sistemas embebidos, combinando características de newlib y glibc.

## Creación de una toolchain con Buildroot y Yocto

### Buildroot
Buildroot permite generar una toolchain personalizada mediante una configuración sencilla:

1. Descargar Buildroot desde su repositorio oficial.
2. Configurar el sistema usando:
   ```sh
   make menuconfig
   ```
3. Seleccionar la opción de toolchain deseada (interno o externo).
4. Compilar la toolchain:
   ```sh
   make
   ```
5. Los binarios resultantes se almacenan en `output/host/bin/`.

### Yocto Project
Yocto genera toolchains personalizadas con gran flexibilidad:

1. Configurar el entorno:
   ```sh
   source oe-init-build-env
   ```
2. Modificar `local.conf` para definir la arquitectura y toolchain deseada.
3. Generar el SDK:
   ```sh
   bitbake core-image-minimal -c populate_sdk
   ```
4. Instalar la toolchain generada en `tmp/deploy/sdk/`.

Tanto Buildroot como Yocto permiten generar toolchains optimizadas para sistemas embebidos, facilitando el desarrollo de software y garantizando compatibilidad con el hardware destino.


