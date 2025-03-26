# Tema 5: Bootloader

## Concepto de Bootloader y su función en sistemas embebidos
El **bootloader** es un software que se ejecuta inmediatamente después de que el procesador se activa tras un reinicio o encendido. Su función principal es inicializar el hardware esencial y cargar el kernel del sistema operativo en la memoria RAM. En sistemas embebidos, el bootloader es crucial ya que permite:

- Configurar el procesador y la memoria.
- Cargar el kernel desde una memoria externa (Flash, SD, eMMC, etc.).
- Pasar parámetros de arranque al sistema operativo.
- Ofrecer interfaces de recuperación y actualización.

Ejemplos de bootloaders incluyen **U-Boot, Barebox y GRUB**.

## Secuencia de arranque en Linux embebido
El arranque de un sistema Linux embebido sigue estos pasos:

1. **Ejecución del código de arranque en ROM**: El procesador comienza la ejecución de un código almacenado en ROM (código de arranque primario).
2. **Carga del Bootloader**: Se carga el bootloader desde una memoria secundaria (NOR/NAND Flash, eMMC, SD).
3. **Inicialización del hardware**: Se configuran los relojes, DRAM y otros periféricos esenciales.
4. **Carga del Kernel de Linux**: El bootloader ubica el kernel en RAM y le transfiere el control.
5. **Paso de parámetros al kernel**: Se proporciona una línea de comandos de arranque y, opcionalmente, un **Device Tree Blob (DTB)**.
6. **Ejecución del kernel**: El kernel de Linux inicia su secuencia de inicialización y monta el sistema de archivos raíz.

## Introducción a U-Boot y su arquitectura
U-Boot (Das U-Boot) es el bootloader más utilizado en sistemas embebidos y soporta múltiples arquitecturas como **ARM, PowerPC, MIPS y x86**.

### Componentes principales de U-Boot
- **SPL (Secondary Program Loader)**: Carga la siguiente etapa del bootloader si la memoria RAM aún no está configurada.
- **TPL (Tertiary Program Loader)** (opcional): Se usa en algunos sistemas para cargar el bootloader principal.
- **Bootloader principal**: Proporciona comandos interactivos, configura el hardware y carga el sistema operativo.

U-Boot permite cargar el kernel desde diferentes medios (Flash, SD, eMMC, Red mediante TFTP/NFS).

## Configuración y compilación de U-Boot
Para configurar y compilar U-Boot:

### Obtener el código fuente
```bash
git clone https://source.denx.de/u-boot/u-boot.git
cd u-boot
```

### Seleccionar la configuración del hardware (ejemplo para BeagleBone Black)
```bash
make CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_defconfig
```

### Compilar U-Boot
```bash
make CROSS_COMPILE=arm-linux-gnueabihf-
```

### Instalar en una SD
```bash
dd if=MLO of=/dev/sdX bs=512 seek=1
dd if=u-boot.img of=/dev/sdX bs=512 seek=2
```

**Nota:** Ver manual de rerencia del SoC ([am335x_reference_manual](../assets/am335x_reference_manual.pdf))

## Variables de entorno y configuración en U-Boot
Las variables de entorno en U-Boot se almacenan en memoria Flash y pueden ser editadas para personalizar el arranque.

### Ver variables de entorno
```bash
printenv
```

### Modificar una variable
```bash
setenv bootcmd "bootm 0x82000000"
```

### Guardar la configuración en memoria Flash
```bash
saveenv
```

Las variables pueden definir métodos de arranque, configurar interfaces de red, establecer opciones del kernel, etc.

## Métodos de carga del Kernel desde U-Boot
U-Boot puede cargar el kernel desde múltiples fuentes:

### Desde NAND Flash
```bash
nand read 0x82000000 0x280000 0x400000
```

### Desde tarjeta SD
```bash
fatload mmc 0:1 0x82000000 zImage
```

### Desde red (TFTP)
```bash
setenv serverip 192.168.1.1
setenv ipaddr 192.168.1.100
tftp 0x82000000 zImage
```

### Ejecutar el kernel
```bash
bootz 0x82000000 - 0x83000000
```

## Soporte para almacenamiento Flash en U-Boot
U-Boot soporta varios tipos de memoria Flash:

- **NOR Flash**: Se puede mapear directamente en la memoria.
- **NAND Flash**: Se accede mediante controladores de memoria MTD.
- **eMMC/SD**: Se gestiona mediante comandos MMC.

Ejemplo de lectura y escritura en NAND:
```bash
nand erase 0x200000 0x400000
nand write 0x82000000 0x200000 0x400000
```

## Uso de Device Tree en U-Boot
Device Tree describe el hardware del sistema y es utilizado tanto por el bootloader como por el kernel.

### Cargar un Device Tree Blob (DTB)
```bash
fatload mmc 0:1 0x83000000 am335x-boneblack.dtb
```

### Pasar el DTB al kernel
```bash
bootz 0x82000000 - 0x83000000
```

## Depuración y recuperación del Bootloader
En caso de fallos en el arranque, se pueden realizar diagnósticos mediante el puerto serie y comandos de U-Boot.

### Reinstalación de U-Boot en memoria Flash
1. Ingresar a U-Boot por consola serie.
2. Cargar U-Boot desde TFTP o SD.
3. Escribir en NAND o eMMC.

Si el bootloader no responde, se puede recurrir a métodos como **JTAG** o **modo de recuperación USB**.

## Alternativas a U-Boot: Barebox, GRUB
Existen otras opciones además de U-Boot:

- **Barebox**: Similar a U-Boot, pero con mejor integración con Device Tree y menos consumo de memoria.
- **GRUB**: Usado en arquitecturas x86 y algunas plataformas embebidas con BIOS/UEFI.

## Ejecución de Aplicación Bare-Metal
Algunas aplicaciones pueden ejecutarse sin un sistema operativo, directamente desde el bootloader.

Ejemplo de ejecución de código bare-metal en U-Boot:
```bash
go 0x82000000
```
Donde `0x82000000` es la dirección donde se cargó el binario ejecutable.

## Conclusión
El bootloader es una parte fundamental del arranque de Linux embebido. **U-Boot** es la opción más popular debido a su flexibilidad y soporte para múltiples arquitecturas. **Configurar adecuadamente el bootloader** es esencial para garantizar una correcta inicialización del sistema y optimizar los tiempos de arranque.


