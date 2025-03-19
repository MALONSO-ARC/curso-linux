# Tema 2: Entorno de Desarrollo en Linux Embebido

## Arquitecturas de CPU en sistemas embebidos

Los sistemas embebidos abarcan una gran variedad de aplicaciones que van desde dispositivos de consumo hasta sistemas industriales de misión crítica. La elección de la arquitectura de CPU es una decisión clave que influye en el rendimiento, consumo energético, compatibilidad con software y herramientas de desarrollo disponibles. En esta sección exploraremos las principales arquitecturas utilizadas en sistemas embebidos con Linux, sus características y consideraciones al seleccionarlas.

### **Clasificación de Arquitecturas en Sistemas Embebidos**
Las CPU utilizadas en sistemas embebidos pueden clasificarse según diferentes criterios:

#### **Por tipo de conjunto de instrucciones (ISA, Instruction Set Architecture)**
- **CISC (Complex Instruction Set Computing)**: Conjuntos de instrucciones complejos, con instrucciones especializadas para tareas de alto nivel (ej. x86).
- **RISC (Reduced Instruction Set Computing)**: Conjuntos de instrucciones más simples y optimizados para eficiencia energética y velocidad (ej. ARM, MIPS, RISC-V).

#### **Por presencia de unidad de gestión de memoria (MMU)**
- **Con MMU**: Soportan sistemas operativos completos como Linux convencional (ej. ARM Cortex-A, x86, MIPS).
- **Sin MMU**: Utilizan variantes reducidas como **uClinux** (ej. ARM Cortex-M, Blackfin, MicroBlaze).

#### **Por soporte de paralelismo y múltiples núcleos**
- **Single-core**: CPU con un solo núcleo de ejecución.
- **Multi-core**: CPU con varios núcleos, común en dispositivos modernos para mejorar el rendimiento y la eficiencia energética.

#### **Por compatibilidad con Floating Point Unit (FPU)**
- **Soft-float**: Sin hardware dedicado para cálculos de punto flotante (procesan en software).
- **Hard-float**: Poseen una FPU para cálculos matemáticos más rápidos.

---

### **Principales Arquitecturas de CPU en Linux Embebido**

#### **ARM (Advanced RISC Machine)**
- **Popularidad**: La arquitectura más utilizada en sistemas embebidos debido a su eficiencia energética y amplio soporte de software.
- **Variantes principales**:
  - **Cortex-A**: Diseñada para aplicaciones con Linux convencional, como smartphones y SBCs (ej. Raspberry Pi).
  - **Cortex-R**: Usada en sistemas en tiempo real, con enfoque en seguridad y fiabilidad.
  - **Cortex-M**: Para microcontroladores sin MMU, compatibles con uClinux en algunos casos.
- **Endianness**: Puede operar en modos big-endian y little-endian.
- **ABI y Floating Point**:
  - Usa **EABI** y su variante **EABIHF** (para FPU) en las versiones modernas.

#### **MIPS (Microprocessor without Interlocked Pipeline Stages)**
- **Históricamente usada** en routers, sistemas de red y aplicaciones industriales.
- **Endianness**: Puede operar en big-endian (ej. "mipseb") o little-endian (ej. "mipsel").
- **Desafíos**:
  - En desuso en comparación con ARM, con menos soporte en nuevas versiones del kernel.
  - Arquitecturas antiguas dependen de toolchains y versiones de software específicas.

#### **x86 y x86_64 (Intel/AMD)**
- **Usos en embebidos**: Encontrado en SBCs industriales y sistemas que requieren compatibilidad con software de PC.
- **Ventajas**:
  - Gran compatibilidad con software existente.
  - Alto rendimiento.
- **Desventajas**:
  - Mayor consumo energético comparado con ARM y RISC-V.
  - No es la opción más común en dispositivos de bajo consumo.

#### **PowerPC (PPC)**
- **Uso tradicional** en telecomunicaciones, industria automotriz y sistemas de defensa.
- **Fabricantes principales**: NXP/Freescale.
- **Consideraciones**:
  - Soporte en declive en el kernel de Linux.
  - Más eficiente que x86 en ciertos escenarios, pero menos adoptado que ARM.

#### **RISC-V**
- **Arquitectura abierta** sin royalties, impulsada por la comunidad open-source.
- **Ventajas**:
  - Flexible y escalable (desde microcontroladores hasta sistemas de alto rendimiento).
  - En crecimiento dentro del ecosistema embebido.
- **Desafíos**:
  - Menos maduro en términos de herramientas de desarrollo y soporte de software comparado con ARM y x86.
  - Algunas toolchains y toolsets aún en desarrollo.


### **Consideraciones en la Selección de la Arquitectura**

#### **Compatibilidad con Toolchain**
- Cada arquitectura requiere un toolchain específico para compilar software.
- GNU usa un prefijo para herramientas de compilación basado en la arquitectura:
  - **Ejemplo para ARM:** `arm-linux-gnueabihf-gcc`
  - **Ejemplo para MIPS:** `mipsel-linux-gnu-gcc`
  - **Ejemplo para RISC-V:** `riscv64-linux-gnu-gcc`

#### **Soporte del Kernel de Linux**
- Las arquitecturas compatibles con el kernel incluyen: ARM, x86, MIPS, PowerPC y RISC-V.
- Algunas arquitecturas sin MMU requieren variantes como **uClinux**.

#### **Consumo Energético y Eficiencia**
- **Sistemas de bajo consumo**: ARM, RISC-V.
- **Sistemas de alto rendimiento**: x86_64, PowerPC.

#### **Disponibilidad de Hardware y Comunidad**
- **ARM** domina el mercado embebido con gran soporte de comunidad y hardware.
- **RISC-V** está en crecimiento pero con menor adopción industrial.
- **MIPS** y **PowerPC** están en declive en términos de nuevas implementaciones.


### Diferencias entre arquitecturas de 32 y 64 bits
| Característica | 32 bits | 64 bits |
|--------------|---------|---------|
| Espacio de direcciones | 4GB máximo | Teóricamente ilimitado |
| Uso de memoria | Menos eficiente en grandes volúmenes de RAM | Mejor gestión de memoria |
| Instrucciones | Menos complejas | Mayor capacidad de cómputo |

[Instruction set ARM64](https://developer.arm.com/documentation/ddi0602/2024-12/?lang=en)

### **Conclusión**
La elección de la arquitectura de CPU en sistemas embebidos depende de múltiples factores, incluyendo el consumo energético, compatibilidad con software, herramientas de desarrollo y requerimientos de rendimiento. ARM sigue siendo el estándar en sistemas embebidos modernos, pero otras arquitecturas como RISC-V están emergiendo como alternativas viables.

Para proyectos de Linux embebido, es fundamental seleccionar una arquitectura bien soportada por el kernel y las toolchains existentes, asegurando compatibilidad a largo plazo y optimización de recursos del sistema.

## Memoria en sistemas embebidos

La memoria es un recurso fundamental en sistemas embebidos, ya que determina la capacidad del sistema para ejecutar aplicaciones, almacenar datos y manejar tareas en tiempo real. A diferencia de los sistemas de propósito general, los sistemas embebidos tienen restricciones de consumo energético, costo y espacio, lo que influye en la elección y administración de la memoria.

## Tipos de Memoria

### RAM (Random Access Memory)
La RAM es utilizada para la ejecución de programas y almacenamiento temporal de datos. Existen varios tipos en sistemas embebidos:

- **SRAM (Static RAM)**: Memoria de acceso rápido y bajo consumo energético, pero costosa y de baja capacidad. Usada en caché y buffers de alta velocidad.
- **DRAM (Dynamic RAM)**: Más económica y de mayor capacidad que la SRAM, pero requiere refresco constante.
- **SDRAM (Synchronous DRAM)**: Sincronizada con el reloj del sistema, mejora la eficiencia en comparación con la DRAM tradicional.
- **DDR (Double Data Rate)**: Versiones DDR, DDR2, DDR3 y DDR4 ofrecen mayores velocidades y menor consumo por bit transferido.
- **LPDDR (Low Power DDR)**: Variante de DDR optimizada para bajo consumo en dispositivos móviles y sistemas embebidos.

### ROM (Read-Only Memory)
La ROM almacena firmware y código esencial que no cambia con frecuencia. Tipos comunes incluyen:

- **Masked ROM**: Programada en fábrica y no modificable.
- **PROM (Programmable ROM)**: Puede programarse una sola vez.
- **EPROM (Erasable PROM)**: Puede borrarse mediante exposición a luz ultravioleta.
- **EEPROM (Electrically Erasable PROM)**: Permite escritura y borrado eléctrico, aunque con ciclos de vida limitados.

### Memoria Flash
Almacenamiento no volátil utilizado para firmware, sistemas de archivos y datos persistentes. Existen dos tipos principales:

- **NOR Flash**: Acceso aleatorio, adecuada para código ejecutable.
- **NAND Flash**: Más densa y rápida, utilizada en almacenamiento masivo (eMMC, SD, SSD).

## Administración de Memoria en Linux Embebido


### Sistemas de Archivos basados en RAM
Sistemas embebidos suelen usar sistemas de archivos optimizados para RAM y almacenamiento Flash:

#### tmpfs (Temporary File System)
- Almacena archivos en RAM de manera temporal.
- Su tamaño puede ajustarse dinámicamente según la cantidad de RAM disponible.
- Se utiliza comúnmente para `/tmp`, `/var/run`, `/var/lock` y otras áreas de datos temporales.
- Se monta con el comando:
  ```bash
  mount -t tmpfs -o size=100M tmpfs /mnt/tmp
  ```
- Se puede definir en `/etc/fstab` para montarse automáticamente al inicio:
  ```
  tmpfs /tmp tmpfs defaults,size=100M 0 0
  ```

#### ramfs
- Similar a `tmpfs`, pero sin límites de tamaño configurables.
- Puede crecer indefinidamente hasta consumir toda la RAM, lo que puede causar problemas si no se controla.
- Se monta con:
  ```bash
  mount -t ramfs ramfs /mnt/ramdisk
  ```

#### zram (Comprimido en RAM)
- Utiliza compresión para almacenar más datos en la misma cantidad de RAM.
- Se usa comúnmente para `swap` comprimido, mejorando el rendimiento en sistemas con poca RAM.
- Se activa con:
  ```bash
  modprobe zram
  echo 512M > /sys/block/zram0/disksize
  ```

#### zswap
- Mecanismo de compresión de páginas de memoria en `swap`, reduciendo accesos a disco.
- No es un sistema de archivos, sino una optimización del subsistema de memoria virtual de Linux.
- Se activa con:
  ```bash
  echo 1 > /sys/module/zswap/parameters/enabled

### Sistemas de Ficheros para Memorias Flash en Linux

Las memorias Flash presentan características únicas, como ciclos de escritura limitados y la necesidad de gestión de bloques defectuosos. Linux cuenta con varios sistemas de archivos optimizados para estos dispositivos, asegurando una mayor durabilidad y eficiencia.

#### JFFS2 (Journaling Flash File System v2)
- Diseñado para memorias **NOR Flash**.
- Soporta **wear leveling** para distribuir escrituras y prolongar la vida útil.
- Maneja **compresión de datos** para optimizar el almacenamiento.
- Adecuado para sistemas con escritura poco frecuente.
- Montaje en Linux:
  ```bash
  mount -t jffs2 /dev/mtdblock0 /mnt/flash
  ```

#### YAFFS2 (Yet Another Flash File System v2)
- Diseñado específicamente para **NAND Flash**.
- Optimizado para rendimiento en escritura.
- Soporta **detección y corrección de errores (ECC)**.
- Recomendado para sistemas con muchas escrituras, como dispositivos móviles.
- Montaje en Linux:
  ```bash
  mount -t yaffs2 /dev/mtdblock0 /mnt/flash
  ```

#### UBIFS (Unsorted Block Image File System)
- Diseñado para **NAND Flash**, reemplazando a JFFS2 en muchas aplicaciones.
- Mejora la escalabilidad y el rendimiento en comparación con JFFS2.
- Usa **UBI (Unsorted Block Images)** para gestionar bloques de memoria.
- Permite **compresión de datos** y mejor manejo de cortes de energía.
- Montaje en Linux:
  ```bash
  ubiattach -m 0 -d 0
  mount -t ubifs ubi0:rootfs /mnt/flash
  ```

#### F2FS (Flash-Friendly File System)
- Diseñado para memorias **eMMC, SSD y tarjetas SD**.
- Optimizado para dispositivos de almacenamiento de alta velocidad.
- Minimiza el desgaste mediante un algoritmo de **garbage collection eficiente**.
- Montaje en Linux:
  ```bash
  mkfs.f2fs /dev/mmcblk0p1
  mount -t f2fs /dev/mmcblk0p1 /mnt/flash
  ```

### Consideraciones en la Selección de un Sistema de Ficheros para Flash
- **NOR Flash**: JFFS2 es una opción estable, aunque UBIFS es preferible en sistemas modernos.
- **NAND Flash**: YAFFS2 y UBIFS son las mejores opciones según el uso.
- **eMMC, SSD, Tarjetas SD**: F2FS es la mejor alternativa debido a su rendimiento y optimización para almacenamiento de bloques.

La elección del sistema de archivos adecuado depende del tipo de memoria Flash utilizada y los requisitos de rendimiento, durabilidad y confiabilidad del sistema embebido.


### Técnicas de Optimización
Dado que la memoria en sistemas embebidos es limitada, se aplican diversas estrategias de optimización:

- **Compresión de memoria**: Uso de zswap o zram para reducir la presión sobre RAM.
- **Memoria compartida**: Permite que múltiples procesos utilicen regiones de memoria comunes.
- **Wear Leveling**: Distribuye escrituras equitativamente en memoria Flash para extender su vida útil.
- **Recolección de basura en NAND**: Minimiza el impacto del borrado de bloques en memoria Flash.

## Consideraciones en la Selección de Memoria

### Consumo Energético
Sistemas embebidos deben balancear rendimiento y eficiencia energética:

- **LPDDR y SRAM** para bajo consumo.
- **NAND Flash** de bajo voltaje en dispositivos móviles.
- **Desactivar caché innecesaria** para ahorrar energía en sistemas de baja potencia.

### Durabilidad y Ciclo de Vida
Memorias Flash tienen ciclos de escritura limitados, lo que requiere:

- **Algoritmos de wear leveling** para distribuir escrituras.
- **Detección y corrección de errores (ECC)** en NAND Flash.

### Costos y Espacio
El tipo y cantidad de memoria deben ajustarse a los costos del hardware embebido:

- **SRAM es costosa pero rápida**, adecuada para pequeños buffers.
- **DRAM es más barata**, pero consume más energía.
- **NAND Flash ofrece más almacenamiento** a menor costo que NOR.

### Conclusión
La gestión eficiente de la memoria en sistemas embebidos es crucial para garantizar rendimiento, estabilidad y durabilidad. La elección de la memoria adecuada depende de los requisitos del sistema, incluyendo consumo energético, velocidad, costo y confiabilidad. Linux embebido ofrece múltiples mecanismos para administrar y optimizar el uso de memoria, lo que permite diseñar soluciones robustas y eficientes.


## Interfaces de hardware en sistemas embebidos
Los sistemas embebidos se comunican con sensores y periféricos mediante diversas interfaces:

- **GPIO (General Purpose Input/Output)**: Puntos de E/S programables.
- **I2C (Inter-Integrated Circuit)**: Bus de comunicación de dos hilos para sensores y dispositivos de baja velocidad.
- **SPI (Serial Peripheral Interface)**: Protocolo rápido para conectar memorias y periféricos.
- **UART (Universal Asynchronous Receiver-Transmitter)**: Comunicación serie, usada para depuración.
- **CAN (Controller Area Network)**: Comunicación en tiempo real para automoción e industria.
- **PCIe (Peripheral Component Interconnect Express)**: Conexión de alta velocidad para hardware avanzado.
- **USB (Universal Serial Bus)**: Uso en almacenamiento, periféricos y comunicación.

## Entrada y salida en sistemas embebidos
Los sistemas embebidos manejan diversos dispositivos de entrada/salida:

- **Sensores**: Captan información del entorno (temperatura, aceleración, etc.).
- **Pantallas**: LCD, OLED o ePaper para visualización.
- **Teclados y botones**: Entrada de usuario en sistemas interactivos.
- **Actuadores**: Motores, servos, relés para control físico.

## Tipos de buses y protocolos en Linux embebido
Linux embebido soporta múltiples buses de comunicación:

| Bus | Características |
|-----|---------------|
| **I2C** | Bajo consumo, hasta 400 kHz, soporta múltiples dispositivos. |
| **SPI** | Comunicación rápida, full-duplex, común en memorias y pantallas. |
| **UART** | Serial, asíncrono, usado para depuración y comunicación con periféricos. |
| **CAN** | Usado en automoción e industria, resistente a ruido. |
| **PCIe** | Alta velocidad, usado en hardware avanzado. |
| **USB** | Versátil, usado para almacenamiento y conectividad. |

## Comunicación con hardware desde Linux
Linux embebido permite interactuar con hardware mediante:

- **Archivos de dispositivo (`/dev`)**: Representan periféricos como archivos.
- **`sysfs`**: Exposición de información del hardware en `/sys`.
- **`mmap`**: Acceso directo a memoria de hardware. (No recomendado)

Ejemplo de control de un GPIO en Linux:
```bash
echo 17 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio17/direction
echo 1 > /sys/class/gpio/gpio17/value  # Enciende el GPIO
echo 0 > /sys/class/gpio/gpio17/value  # Apaga el GPIO
```

## Herramientas de monitoreo de hardware en Linux embebido
Algunas herramientas esenciales para diagnosticar y monitorear hardware:

- **`i2cdetect`**: Escaneo de dispositivos I2C.
- **`spidev_test`**: Prueba de comunicación SPI.
- **`gpioinfo`**: Información de pines GPIO.
- **`dmesg`**: Registro de eventos del kernel.

## Flujos de trabajo recomendados para desarrollo en Linux embebido

El desarrollo en Linux embebido requiere un flujo de trabajo bien estructurado para optimizar el tiempo y minimizar errores en el proceso. A continuación, se presentan los pasos recomendados para un flujo de trabajo eficiente:

### Configuración del entorno
Para comenzar, es fundamental preparar una máquina de desarrollo con todas las herramientas necesarias:
- Instalar una **cross-toolchain** adecuada para la arquitectura del sistema embebido (por ejemplo, `arm-linux-gnueabihf-gcc` para ARM).
- Configurar un entorno de compilación como **Buildroot** o **Yocto** para generar el sistema base.
- Instalar herramientas como `make`, `cmake`, `gcc`, `binutils` y `gdb`.
- Utilizar un emulador como **QEMU** para pruebas sin necesidad de hardware físico.

### Compilación cruzada
Dado que los sistemas embebidos suelen tener recursos limitados, es común realizar la compilación en una máquina host:
- Usar un compilador cruzado (`arm-linux-gnueabihf-gcc`, `riscv64-linux-gnu-gcc`, etc.) para generar ejecutables para la plataforma objetivo.
- Definir correctamente el **sysroot** y las opciones de compilación para evitar problemas de dependencias.
- Utilizar herramientas como **CMake** o **Autotools** para gestionar la compilación de proyectos complejos.

Ejemplo de compilación cruzada:
```bash
arm-linux-gnueabihf-gcc -o programa_embebido programa.c
```

### Flasheo de imágenes
Una vez compilado el software, es necesario cargarlo en el dispositivo embebido:
- Para sistemas que utilizan tarjetas SD o eMMC, herramientas como `dd` o **Balena Etcher** permiten grabar imágenes de sistema.
- Para dispositivos con memoria Flash SPI o NAND, se pueden usar herramientas como `flashrom` o `mtd-utils`.
- En sistemas más avanzados, es posible usar U-Boot y TFTP para cargar imágenes a través de la red.

Ejemplo de flasheo de una imagen en una tarjeta SD:
```bash
sudo dd if=imagen_linux.img of=/dev/sdX bs=4M status=progress
sync
```

### Pruebas y depuración
El proceso de desarrollo requiere herramientas de diagnóstico para identificar y solucionar errores:
- **GDB**: Depuración de programas en el sistema embebido.
- **strace**: Permite analizar llamadas al sistema y detectar fallos en la ejecución.
- **dmesg**: Inspecciona mensajes del kernel, útil para detectar fallos en drivers o hardware.
- **valgrind**: Detecta fugas de memoria en aplicaciones embebidas.

Ejemplo de depuración con GDB en un sistema embebido:
```bash
gdbserver :1234 ./programa_embebido
```
Luego, en la máquina host:
```bash
gdb-multiarch ./programa_embebido
(gdb) target remote <ip>:1234
```

### Automatización
Para mejorar la eficiencia, muchas tareas pueden automatizarse con **scripting**:
- Uso de **Bash** o **Python** para generar builds automatizados.
- Implementación de scripts para flasheo rápido de imágenes.
- Integración con sistemas de CI/CD como **Gitlab** o **GitHub Actions**.
- Automatización de pruebas con frameworks específicos.

Ejemplo de script en Bash para compilar y copiar un ejecutable a un dispositivo embebido:
```bash
#!/bin/bash
arm-linux-gnueabihf-gcc -o programa programa.c && scp programa usuario@192.168.1.100:/home/usuario/
```

### Conclusión
Siguiendo estos flujos de trabajo, el desarrollo en Linux embebido se vuelve más estructurado, eficiente y reproducible. Implementar estas prácticas facilita la depuración, reduce errores y permite una mejor integración del software en sistemas embebidos reales.

