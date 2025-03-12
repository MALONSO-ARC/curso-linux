# Tema 6: Linux Kernel

## ¿Qué es el Kernel de Linux y su función en un sistema embebido?
El **Kernel** es el núcleo del sistema operativo, encargado de administrar hardware y recursos.

## Arquitectura interna del Kernel
El Kernel de Linux está compuesto por:
- **Manejo de procesos**
- **Sistema de archivos**
- **Administración de memoria**
- **Gestión de dispositivos**

## Diferencias entre Kernel monolítico y modular
- **Monolítico**: Todos los controladores están en el mismo binario.
- **Modular**: Los controladores pueden cargarse y descargarse dinámicamente.

## Configuración y compilación de un Kernel embebido
Ejemplo de configuración y compilación:
```bash
make ARCH=arm menuconfig
make -j4
```

## Introducción a Device Tree y su uso en hardware embebido
El **Device Tree** describe el hardware para el Kernel.

## Carga y gestión de módulos del Kernel
```bash
insmod modulo.ko  # Cargar módulo
rmmod modulo      # Descargar módulo
```

## Manejo de interrupciones y controladores de dispositivos
Las interrupciones permiten responder a eventos de hardware.

## Soporte de hardware en el Kernel: drivers y frameworks
El Kernel incluye **drivers** para múltiples dispositivos.

## Optimización del Kernel para sistemas embebidos
- **Reducir tamaño** eliminando funciones innecesarias.
- **Optimizar consumo energético**.

## Booting del Kernel y análisis de logs (`dmesg`)
```bash
dmesg | grep error  # Ver errores de arranque
```

---
**Fin del Tema 6**
