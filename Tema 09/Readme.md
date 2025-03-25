# Tema9: Board Support Package (BSP) en Linux Embebido

## **Introducción al concepto de BSP y su rol en Linux embebido**
El **Board Support Package (BSP)** es un conjunto de software y configuraciones necesarias para ejecutar un sistema operativo en un hardware específico. En el contexto de **Linux embebido**, el BSP es crucial para garantizar la compatibilidad entre el sistema operativo y la plataforma de hardware.

El BSP en Linux embebido típicamente incluye:
- **Bootloader**: Inicializa el hardware y carga el kernel.
- **Kernel de Linux**: Adaptado para el hardware específico.
- **Drivers**: Permiten la comunicación con los periféricos.
- **Root Filesystem (RootFS)**: Contiene los programas y bibliotecas necesarias para la ejecución del sistema.

### **Ejemplo práctico**: Uso de BSP en una BeagleBone Black

```bash
# Verificar la información del hardware en una BeagleBone Black
cat /proc/cpuinfo
``` 

## **Componentes de un BSP**
### **Bootloader**
El bootloader es el primer software en ejecutarse y carga el kernel de Linux. Un ejemplo común en sistemas embebidos es **U-Boot**.

Ejemplo de configuración de U-Boot:
```bash
# Configuración del bootloader en U-Boot
setenv bootargs console=ttyO0,115200n8 root=/dev/mmcblk0p2 rw
saveenv
boot
```

### **Kernel de Linux**
El kernel de Linux en un BSP debe estar configurado para el hardware específico de la tarjeta. Para compilarlo:
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage
make dtbs
```

### **Drivers y módulos**
Los drivers son esenciales para el soporte de hardware. Se pueden compilar como módulos del kernel:
```bash
make modules
make modules_install
```

### **Root Filesystem (RootFS)**
El RootFS contiene las aplicaciones y bibliotecas necesarias. Puede generarse con **Buildroot** o **Yocto**.

Ejemplo con BusyBox:
```bash
make menuconfig
make install
```

## **Creación de un BSP personalizado**
Para crear un BSP adaptado a un hardware específico, se deben seguir los siguientes pasos:
1. Configurar y compilar el bootloader (U-Boot o Barebox).
2. Configurar y compilar el kernel con soporte para el hardware.
3. Agregar controladores específicos.
4. Construir un RootFS mínimo.
5. Generar la imagen final del sistema.

## **Uso de meta-layers en Yocto para BSP**
Yocto Project organiza el soporte de hardware mediante **meta-layers**.

Ejemplo de creación de una capa BSP en Yocto:
```bash
cd poky
source oe-init-build-env
bitbake-layers create-layer meta-custom-bsp
```

## **Añadir drivers y módulos personalizados en el BSP**
Para añadir un driver al kernel:
```bash
cd drivers/
mkdir my_driver
cd my_driver
echo "obj-m := my_driver.o" > Makefile
make -C /lib/modules/$(uname -r)/build M=$(pwd) modules
```

## **Integración de Kernel y Bootloader en BSP**
Para integrar el kernel y el bootloader en la imagen final, se pueden usar herramientas como **Yocto, Buildroot o OpenEmbedded**.

Ejemplo de configuración en Yocto:
```bash
echo "IMAGE_INSTALL += \"kernel-modules u-boot\"" >> conf/local.conf
bitbake core-image-minimal
```

## **Creación de imágenes de sistema con un BSP propio**
Para generar una imagen final, se usa **dd** o herramientas específicas del BSP:
```bash
sudo dd if=my_bsp_image.img of=/dev/sdX bs=4M status=progress
```

## **Soporte para múltiples plataformas con un BSP**
El BSP puede adaptarse a múltiples plataformas utilizando device trees (DTB) y compilar diferentes configuraciones.
```bash
make dtbs
```

## **Debugging de BSP y estrategias de mantenimiento**
Algunas herramientas de depuración incluyen:
- **dmesg**: Para verificar mensajes del kernel.
- **strace**: Para rastrear llamadas al sistema.
- **gdb**: Para depurar el kernel y aplicaciones.

Ejemplo:
```bash
dmesg | grep error
```

## **Ejemplo práctico: Creación de un BSP para BeagleBone**
### **Pasos**:
1. Obtener y compilar el bootloader (U-Boot).
2. Configurar y compilar el kernel.
3. Crear un RootFS con BusyBox.
4. Generar una imagen para la tarjeta SD.

```bash
bitbake core-image-minimal
```
