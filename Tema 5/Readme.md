# Tema 5: Bootloader

## Concepto de Bootloader y su función en sistemas embebidos
El **bootloader** es un programa que se ejecuta al encender un dispositivo y carga el sistema operativo.

## Secuencia de arranque en Linux embebido
1. **ROM Code**: Primera fase, grabada en el chip.
2. **Bootloader**: Carga el kernel de Linux.
3. **Kernel**: Inicializa los dispositivos.
4. **RootFS**: Monta el sistema de archivos.

## Introducción a U-Boot y su arquitectura
U-Boot es el bootloader más usado en Linux embebido.

## Configuración y compilación de U-Boot
Ejemplo de compilación para ARM:
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- u-boot.bin
```

## Variables de entorno y configuración en U-Boot
```bash
printenv
setenv bootcmd "run boot_linux"
saveenv
```

## Métodos de carga del Kernel desde U-Boot
- **TFTP**: Red.
- **SD/eMMC**: Almacenamiento.
- **NAND/SPI Flash**: Memoria integrada.

## Uso de Device Tree en U-Boot
El **Device Tree** describe el hardware al kernel.

## Depuración y recuperación del Bootloader
- **JTAG** para acceso de bajo nivel.
- **U-Boot interactivo** para cambiar parámetros.

## Alternativas a U-Boot
- **Barebox**
- **GRUB**

---
**Fin del Tema 5**
