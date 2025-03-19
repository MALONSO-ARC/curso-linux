# Tema 6: LINUX KERNEL

## ¿Qué es el Kernel de Linux y su función en un sistema embebido?
El **Kernel de Linux** es el núcleo del sistema operativo y se encarga de gestionar los recursos de hardware y software en un sistema embebido. Sus funciones principales incluyen:

- Gestión de procesos y multitarea.
- Manejo de memoria.
- Control de dispositivos y gestión de hardware.
- Sistema de archivos.
- Comunicación entre procesos (IPC).

En sistemas embebidos, el kernel debe ser **eficiente y optimizado** para el hardware específico en el que se ejecuta.

## Arquitectura interna del Kernel
El kernel de Linux sigue una arquitectura modular y consta de varios subsistemas clave:

- **Gestión de procesos**: Planificación, sincronización y control de hilos.
- **Gestión de memoria**: Paginación, swapping y asignación de memoria.
- **Sistema de archivos**: Soporte para múltiples sistemas de archivos como ext4, FAT, yaffs, etc.
- **Controladores de dispositivos (drivers)**: Interfaz entre hardware y software.
- **Red y protocolos de comunicación**: TCP/IP, Bluetooth, WiFi, CAN, etc.
- **Seguridad**: Control de permisos, SELinux, control de acceso.

## Diferencias entre Kernel monolítico y modular
El kernel de Linux puede configurarse de dos formas principales:

### Kernel Monolítico
- Todos los controladores y funcionalidades están compilados dentro del kernel.
- Mayor rendimiento, ya que no hay sobrecarga de carga de módulos.
- Difícil de actualizar sin recompilar el kernel completo.

### Kernel Modular
- Permite cargar y descargar módulos en tiempo de ejecución.
- Ocupa menos espacio en memoria al cargar solo los módulos necesarios.
- Facilita la actualización de controladores sin reiniciar el sistema.

Ejemplo de carga de un módulo en un kernel modular:
```bash
sudo modprobe i2c-dev
```

## Configuración y compilación de un Kernel embebido
Para compilar un kernel para un sistema embebido, se sigue este proceso:

### 1. Obtener el código fuente
```bash
git clone https://github.com/torvalds/linux.git
cd linux
```

### 2. Configurar el kernel
Se debe seleccionar la configuración específica para el hardware:
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
```

### 3. Compilar el kernel
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 zImage
dtbs modules
```

### 4. Instalar módulos y kernel
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=/mnt/rootfs modules_install
```

## Introducción a Device Tree y su uso en hardware embebido
El **Device Tree (DT)** es un mecanismo para describir hardware al kernel de Linux sin necesidad de modificar su código fuente.

Ejemplo de un archivo **Device Tree Source (.dts)**:
```dts
/ {
    model = "BeagleBone Black";
    compatible = "ti,am335x-boneblack";

    memory@80000000 {
        device_type = "memory";
        reg = <0x80000000 0x20000000>; /* 512 MB */
    };
};
```

Compilación de un Device Tree:
```bash
dtc -I dts -O dtb -o am335x-boneblack.dtb am335x-boneblack.dts
```

## Carga y gestión de módulos del Kernel
El kernel modular permite cargar y gestionar módulos dinámicamente:

- **Cargar un módulo**:
  ```bash
  sudo insmod mi_modulo.ko
  ```
- **Remover un módulo**:
  ```bash
  sudo rmmod mi_modulo
  ```
- **Ver información de un módulo**:
  ```bash
  modinfo mi_modulo.ko
  ```
- **Listar módulos cargados**:
  ```bash
  lsmod
  ```

## Manejo de interrupciones y controladores de dispositivos
Las interrupciones permiten que los dispositivos notifiquen eventos al procesador. En el kernel de Linux, los controladores de dispositivos manejan las interrupciones mediante IRQs.

Ejemplo de registro de una interrupción en un driver:
```c
static irqreturn_t mi_handler_irq(int irq, void *dev_id) {
    printk(KERN_INFO "Interrupción recibida en IRQ %d\n", irq);
    return IRQ_HANDLED;
}

request_irq(IRQ_NUM, mi_handler_irq, IRQF_TRIGGER_RISING, "mi_dispositivo", NULL);
```

## Soporte de hardware en el Kernel: drivers y frameworks
El soporte de hardware en el kernel de Linux se implementa mediante **drivers y frameworks**.

- **Drivers de carácter**: Para dispositivos como UART, GPIO.
- **Drivers de bloque**: Para discos y memorias flash.
- **Drivers de red**: Para Ethernet, WiFi, Bluetooth.
- **Frameworks**:
  - **I2C, SPI, GPIO, PCI** para comunicación con periféricos.
  - **V4L2 (Video for Linux 2)** para cámaras y video.
  - **ALSA (Advanced Linux Sound Architecture)** para audio.

Ejemplo de un driver de carácter:
```c
static int __init mi_driver_init(void) {
    printk(KERN_INFO "Mi driver cargado\n");
    return 0;
}
static void __exit mi_driver_exit(void) {
    printk(KERN_INFO "Mi driver descargado\n");
}
module_init(mi_driver_init);
module_exit(mi_driver_exit);
```

## Optimización del Kernel para sistemas embebidos
Para reducir el tamaño del kernel y mejorar su rendimiento en sistemas embebidos:

- **Eliminar drivers innecesarios** en `menuconfig`.
- **Usar compresión** en el kernel (`gzip`, `xz`).
- **Reducir el tamaño del sistema de archivos**.
- **Deshabilitar características no utilizadas** (`CONFIG_PRINTK`, `CONFIG_DEBUG_INFO`).

## Booting del Kernel y análisis de logs (`dmesg`)
El análisis del arranque del kernel es clave para depuración y optimización.

### Ver los mensajes del kernel
```bash
dmesg | less
```

### Filtrar mensajes de errores
```bash
dmesg | grep -i error
```

### Seguir los logs en tiempo real
```bash
dmesg -w
```

### Ver la línea de comandos pasada al kernel
```bash
cat /proc/cmdline
```

## Conclusión
El **kernel de Linux** es la pieza fundamental en sistemas embebidos. Su correcta configuración y optimización permite maximizar el rendimiento y compatibilidad con el hardware. El uso de Device Tree, módulos del kernel y técnicas de optimización son esenciales para garantizar un sistema embebido eficiente y funcional.

