# Tema 1: Introducción a Linux Embebido

## ¿Qué es un sistema embebido?
Un **sistema embebido** es un sistema computacional diseñado para realizar una función específica dentro de un dispositivo más grande. A diferencia de las computadoras personales, los sistemas embebidos están optimizados para eficiencia, consumo de energía y rendimiento en tareas concretas.

### Características de un sistema embebido:
- Uso de hardware dedicado y optimizado.
- Funcionalidad específica y predefinida.
- Limitaciones de memoria y procesamiento.
- Bajo consumo de energía.
- Requisitos de **tiempo real** en algunos casos.
- Alta integración con sensores y actuadores.

## Diferencias entre Linux embebido y Linux de escritorio
| Característica      | Linux Embebido       | Linux de Escritorio |
|---------------------|---------------------|---------------------|
| Hardware           | Limitado, específico | Generalista, más potente |
| Consumo de energía | Bajo                 | Mayor |
| Interfaces         | GPIO, UART, SPI, I2C | Teclado, mouse, pantalla |
| Espacio en disco   | Reducido             | Amplio |
| Flexibilidad      | Alta, personalizable | Generalista |

## Comparativa de Linux embebido con otros sistemas
| Sistema         | Características principales |
|---------------|-------------------------|
| **Linux Embebido** | Sistema operativo flexible, multitarea, código abierto, ideal para sistemas complejos. |
| **RTOS (Real-Time OS)** | Pequeño, rápido, con tiempos de respuesta predecibles, usado en sistemas de tiempo real. |
| **Bare-metal** | Sin sistema operativo, código ejecutado directamente en hardware, máximo control y eficiencia. |

## Historia y evolución de Linux embebido
Linux embebido ha evolucionado desde sistemas simples en la década de los 90 hasta convertirse en la base de sistemas avanzados en IoT, automoción y robótica. Algunos hitos importantes:

- **1991**: Nacimiento de Linux (Linus Torvalds).
- **1999**: Primer uso extendido en dispositivos embebidos.
- **2003**: Nace el proyecto **OpenEmbedded**.
- **2010**: Crecimiento en el sector IoT.
- **Actualidad**: Uso en automoción, drones, dispositivos médicos, etc.

## Casos de uso en la industria
Linux embebido se utiliza en diversos sectores:
- **Automoción**: Sistemas de infoentretenimiento, ADAS.
- **IoT**: Dispositivos inteligentes, sensores conectados.
- **Robótica**: Control de robots industriales y autónomos.
- **Aeroespacial**: Satélites y sistemas de navegación.
- **Domótica**: Asistentes inteligentes, cámaras de seguridad.

## Componentes de un sistema embebido
Un sistema embebido basado en Linux está compuesto por:
- **Bootloader**: Carga el sistema operativo.
- **Kernel**: Núcleo del sistema operativo.
- **RootFS (Root Filesystem)**: Sistema de archivos raíz con aplicaciones y bibliotecas.
- **Aplicaciones**: Software específico de la aplicación.

## Requisitos de hardware típicos para Linux embebido
- Procesador: ARM, RISC-V, x86, MIPS.
- Memoria RAM: Desde 32MB hasta varios GB.
- Almacenamiento: SD, eMMC, NAND Flash.
- Interfaces: UART, SPI, I2C, GPIO, USB, Ethernet.

## Ejemplos de hardware popular para Linux embebido
| Plataforma      | CPU          | Uso principal |
|---------------|-------------|--------------|
| BeagleBone    | ARM Cortex-A8 | Desarrollo y prototipos |
| Raspberry Pi  | ARM Cortex-A72 | IoT, educación |
| STM32MP1      | ARM Cortex-A7 | Industrial |
| i.MX8        | ARM Cortex-A53 | Automoción, multimedia |

## Desafíos en el desarrollo de Linux embebido
- **Limitaciones de hardware**: CPU, RAM, almacenamiento reducido.
- **Gestión de energía**: Crucial para dispositivos móviles.
- **Compatibilidad de drivers**: Hardware específico requiere soporte adecuado.
- **Tiempo de arranque**: Optimización para reducir el boot time.

## Herramientas y flujos de trabajo comunes
- **Cross-compilación**: Uso de toolchains cruzadas.
- **Yocto/Buildroot**: Creación de imágenes personalizadas.
- **Depuración**: Uso de GDB, strace, dmesg.
- **Control de versiones**: Git para manejar código y configuraciones.

---
**Fin del Tema 1**
