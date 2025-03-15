# Tema 1 - Introducción a Linux Embebido

## 1. ¿Qué es un sistema embebido? Definición y características

Un sistema embebido es un sistema de computación especializado diseñado para realizar una función específica dentro de un dispositivo más grande. A diferencia de un ordenador de propósito general, los sistemas embebidos están optimizados para tareas concretas y suelen funcionar en entornos con restricciones de recursos. Estos sistemas pueden encontrarse en una amplia gama de dispositivos, desde electrodomésticos hasta sistemas industriales y automóviles.

Las principales características de un sistema embebido incluyen:

- **Dedicación a una tarea específica**: Los sistemas embebidos están diseñados para realizar una función concreta, como gestionar un panel de control en un vehículo o procesar datos de un sensor en una fábrica.
- **Limitaciones de recursos**: A menudo, estos sistemas cuentan con procesadores de baja potencia, memoria limitada y almacenamiento restringido, lo que implica la necesidad de optimizar el software para funcionar en estas condiciones.
- **Alta fiabilidad y estabilidad**: Dado que muchos sistemas embebidos operan en entornos críticos, es esencial que sean altamente confiables y capaces de funcionar durante largos períodos sin fallos.
- **Integración con hardware especializado**: Muchos sistemas embebidos incluyen interfaces para sensores, actuadores, y otras formas de hardware personalizado.
- **Frecuentemente operan en tiempo real**: En aplicaciones donde el tiempo de respuesta es crítico, como en sistemas de control industrial, los sistemas embebidos pueden requerir características de tiempo real.

## 2. Diferencias entre Linux embebido y Linux de escritorio

Linux embebido y Linux de escritorio comparten el mismo núcleo (kernel), pero están diseñados para contextos y propósitos diferentes. Mientras que un sistema Linux de escritorio está optimizado para la ejecución de múltiples aplicaciones interactivas con una interfaz gráfica, un sistema Linux embebido está diseñado para ser eficiente y cumplir con requerimientos específicos.

Algunas diferencias clave son:

- **Recursos de hardware**: Linux embebido se ejecuta en dispositivos con recursos limitados, como microcontroladores o SoCs (System-on-Chip), mientras que Linux de escritorio puede aprovechar procesadores potentes y grandes cantidades de memoria.
- **Flexibilidad**: Linux de escritorio es un sistema generalista, capaz de ejecutar múltiples tipos de software, mientras que Linux embebido se configura para un conjunto reducido de aplicaciones.
- **Interfaz gráfica**: Mientras que Linux de escritorio casi siempre incluye un entorno gráfico como GNOME o KDE, en sistemas embebidos, la presencia de una interfaz gráfica no es obligatoria y en muchos casos se emplean interfaces minimalistas o incluso solo una terminal.
- **Mantenimiento y actualizaciones**: Los sistemas embebidos suelen ser más estáticos y pueden no actualizarse con la misma frecuencia que un sistema de escritorio.

## 3. Comparativa de Linux embebido con otros sistemas (RTOS, Bare-metal)

Para entender mejor las ventajas e inconvenientes de Linux embebido, es útil compararlo con otras alternativas comunes como los sistemas operativos en tiempo real (RTOS) y los sistemas bare-metal.

- **Linux embebido**: Ofrece multitarea, gestión de memoria y una rica colección de controladores y software. Sin embargo, puede no ser lo suficientemente determinista para aplicaciones de tiempo real estrictas.
- **RTOS (Real-Time Operating System)**: Diseñados para garantizar tiempos de respuesta predecibles, los RTOS son ideales para aplicaciones que requieren ejecución en plazos estrictos, como sistemas de control en automoción o robótica.
- **Bare-metal**: No utiliza un sistema operativo, sino que ejecuta código directamente en el hardware. Esto proporciona el máximo control y velocidad, pero aumenta la complejidad del desarrollo y mantenimiento.

## 4. Historia y evolución de Linux embebido

El uso de Linux en sistemas embebidos ha evolucionado significativamente desde sus inicios. En los años 90, los desarrolladores comenzaron a adaptar el kernel de Linux para dispositivos con recursos limitados, dando lugar a proyectos como:

# Evolución de Linux Embebido

| Año  | Hito                                      | Descripción |
|------|-------------------------------------------|-------------|
| **1991** | Lanzamiento de Linux 0.1 | Linus Torvalds publica la primera versión del núcleo de Linux como un sistema operativo de código abierto para PCs IBM. |
| **1995** | Inclusión de soporte para múltiples arquitecturas | Linux versión 1.2 añade soporte para Alpha, i386, MIPS y SPARC, facilitando su portabilidad a dispositivos embebidos. |
| **1996** | Inclusión de arquitecturas m68k y PowerPC | Linux 2.0 amplía el soporte a las arquitecturas m68k y PowerPC, comunes en dispositivos embebidos. |
| **1997** | Proyecto Linux Router (LRP) | Dave Cinege lanza LRP, utilizando BusyBox para crear un Linux arrancable desde disquete que convierte una PC en un router, uno de los primeros proyectos reales de Linux embebido. |
| **1998** | Inicio del proyecto BusyBox | Bruce Perens inicia BusyBox, creando una suite de utilidades Unix ligeras, ideales para sistemas embebidos debido a su reducido tamaño. |
| **1998** | Desarrollo de uClinux | Kenneth Albanowski y D. Jeff Dionne trabajan en uClinux, una versión de Linux para sistemas sin unidad de gestión de memoria (MMU), ampliando el rango de hardware capaz de ejecutar Linux. |
| **1999** | Lanzamiento de ELKS | Se presenta el Subconjunto de Kernel de Linux Integrable (ELKS), diseñado para ordenadores de 16 bits con recursos de memoria limitados, como los sistemas basados en Intel 8086. |
| **2001** | Creación de Buildroot | Nace Buildroot como un framework para automatizar la generación de sistemas Linux embebidos minimizados, facilitando la compilación cruzada y la integración de software. |
| **2003** | Fundación del Consorcio de Linux Embebido (ELC) | Empresas como IBM, Intel, Motorola y Samsung forman el ELC para estandarizar y promover el uso de Linux en sistemas embebidos. |
| **2004** | Asociación de Wind River con Red Hat | Wind River, desarrollador de VxWorks, se asocia con Red Hat para presentar Wind River Linux, entrando en el mercado de dispositivos embebidos con Linux. |
| **2010** | Fundación de Linaro | ARM, Freescale, IBM, Samsung, ST-Ericsson y Texas Instruments anuncian Linaro, enfocada en mejorar el desarrollo de software de código abierto para SoCs avanzados. |
| **2011** | Lanzamiento del Proyecto Yocto | La Fundación Linux lanza el Proyecto Yocto para facilitar la creación de distribuciones de Linux para software embebido e IoT, independientes de la arquitectura de hardware. |


A medida que el hardware se ha vuelto más potente y asequible, Linux embebido ha encontrado aplicaciones en industrias como la automoción, la domótica y la robótica.

## 10. Herramientas y flujos de trabajo comunes

Un sistema Linux embebido siempre requiere los siguientes componentes:
- **Bootloader**: Es el software encargado de iniciar el sistema y cargar el kernel de Linux.
- **Kernel de Linux**: El núcleo del sistema operativo que gestiona el hardware y los procesos.
- **RootFS (Sistema de archivos raíz)**: Contiene las bibliotecas y aplicaciones necesarias para que el sistema funcione.

Para generar estos componentes, existen varios enfoques:

| Método | Características | Ventajas | Inconvenientes |
|--------|---------------|----------|---------------|
| **Manual (Cross Linux From Scratch - CLFS)** | Construcción manual de cada componente del sistema. | Control total sobre el sistema, optimización extrema. | Proceso laborioso y requiere experiencia avanzada. |
| **BusyBox** | Proporciona un sistema minimalista con herramientas básicas de Unix. | Ligero y rápido, ideal para sistemas con recursos limitados. | No incluye herramientas avanzadas o personalización extensa. |
| **Buildroot** | Framework que permite generar un sistema embebido con un conjunto de paquetes predefinidos. | Fácil de usar, rápido de configurar, buena documentación. | Menos flexible que Yocto, menos escalable. |
| **Yocto Project** | Plataforma modular y flexible para crear distribuciones Linux embebidas personalizadas. | Altamente configurable, soporte para múltiples arquitecturas, escalabilidad. | Curva de aprendizaje pronunciada, requiere más recursos de compilación. |

Cada una de estas opciones ofrece diferentes grados de flexibilidad, complejidad y optimización, por lo que la elección depende de los requisitos del sistema embebido que se esté desarrollando.

