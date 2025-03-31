# Tema 11: Control de Memoria en Linux Embebido

## 1. Introducción a la Gestión de Memoria en Linux

La gestión de memoria es una de las funciones más críticas del sistema operativo Linux, especialmente en entornos embebidos donde los recursos son limitados. Linux implementa un sistema de memoria virtual sofisticado que abstrae el hardware físico subyacente y permite una administración eficiente de la memoria.

Las principales características del sistema de memoria en Linux incluyen:
- Soporte para **memoria virtual**, que permite a los procesos tener la ilusión de contar con una memoria contigua y mayor que la física disponible.
- **Paginación por demanda**, que reduce el uso innecesario de memoria cargando páginas solo cuando se necesitan.
- **Asignación dinámica de memoria**, que permite reservar y liberar memoria durante la ejecución del programa.
- **Espacio de intercambio (swap)**, que extiende la memoria RAM utilizando almacenamiento secundario.

En sistemas embebidos, es fundamental entender estos mecanismos para poder adaptar el sistema operativo a las limitaciones del hardware.

---

## 2. Diferencia entre Memoria Física y Virtual

### Memoria Física
Es la memoria RAM real instalada en el sistema. Su tamaño está limitado por el hardware y es compartida por el sistema operativo, drivers y aplicaciones.

### Memoria Virtual
Es un espacio de direcciones abstracto que el sistema operativo proporciona a cada proceso. Cada proceso cree tener acceso exclusivo a toda la memoria virtual disponible.

La **MMU (Memory Management Unit)** traduce las direcciones virtuales a direcciones físicas mediante el uso de tablas de páginas. Esta traducción permite:
- Aislamiento entre procesos.
- Protección de memoria.
- Uso eficiente de la memoria disponible mediante la compartición de páginas comunes (por ejemplo, bibliotecas compartidas).

---

## 3. Espacios de Usuario y Kernel

Linux divide el espacio de memoria en:

### Espacio de Usuario
- Utilizado por las aplicaciones.
- Acceso restringido.
- Protegido contra acceso a memoria del kernel o de otros procesos.

### Espacio del Kernel
- Contiene el código del sistema operativo, drivers y estructuras de datos del sistema.
- Acceso privilegiado.
- Protegido de modificaciones por procesos de usuario.

Esta separación mejora la seguridad y estabilidad del sistema.

### Mecanismos de Comunicación entre Espacios

Aunque los espacios de usuario y kernel están separados, existen mecanismos controlados que permiten compartir o intercambiar datos entre ambos:

#### 1. `copy_from_user()` y `copy_to_user()`
Estas funciones son usadas dentro de controladores del kernel (drivers) para transferir datos de forma segura entre el espacio de usuario y el kernel.

Por ejemplo, un dispositivo implementado con `ioctl` puede utilizar estas funciones para leer o escribir estructuras de datos:
```c
if (copy_from_user(&kdata, udata, sizeof(kdata))) {
    return -EFAULT;
}
```
**Nota** Para ver un ejemplo completo de IOCTL (desde el lado del kernel y desde el lado del userspace) mira [AQUI](../assets/ioctl_example.md)


Estas funciones protegen contra accesos no válidos, verificando que las direcciones pasadas por el usuario sean accesibles y válidas.

#### 2. Acceso directo mediante `/dev/mem`
El archivo especial `/dev/mem` permite a los procesos en espacio de usuario acceder directamente al espacio de direcciones físicas, útil en sistemas embebidos para interactuar con hardware.

Ejemplo para mapear una región física a espacio de usuario:
```c
int fd = open("/dev/mem", O_RDWR);
void *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phys_addr);
```

> ⚠️ Nota: El uso de `/dev/mem` implica riesgos de seguridad y estabilidad. Debe ser restringido a propósitos muy controlados.

---

## 4. Uso de la Memoria en Sistemas Embebidos

En sistemas embebidos, la memoria es uno de los recursos más escasos. Las siguientes estrategias son comunes:
- Reducción del tamaño del kernel mediante configuraciones personalizadas.
- Uso de bibliotecas ligeras (uClibc, musl) en lugar de glibc.
- Eliminación de servicios y demonios innecesarios.
- Vigilancia activa de fugas de memoria.

Es habitual utilizar herramientas como `smem`, `top`, `free` o `cat /proc/meminfo` para monitorear el uso de memoria.


En algunas de las aplicaciones para monitorizar la memoria aparecen algunos conceptos como RSS, PSS o USS. En la siguiente tabla tienes un resumen de su significado

| Métrica | Significado                        | Descripción                                                                 |
|---------|------------------------------------|-----------------------------------------------------------------------------|
| RSS     | Resident Set Size                  | Memoria física actualmente en uso por el proceso, incluyendo memoria compartida. |
| PSS     | Proportional Set Size              | Porción proporcional de la memoria compartida más la memoria privada. Si una página es compartida entre 3 procesos, cada uno cuenta con 1/3 de su tamaño. |
| USS     | Unique Set Size                    | Memoria exclusiva del proceso. No incluye ninguna porción de memoria compartida.  |


---

## 5. Técnicas de Optimización de Memoria en Linux Embebido

### Uso de `strip`
- Elimina los símbolos de depuración de los binarios.
- Reduce considerablemente el tamaño de ejecutables.

### Librerías Compartidas
- Permiten que varios procesos compartan la misma copia en memoria.
- Reduce el consumo de RAM frente a binarios estáticos.

### Configuración del Kernel
- Deshabilitar drivers y subsistemas innecesarios.
- Compilar el kernel a medida.

### ZRAM y SWAP Comprimido
- ZRAM crea un dispositivo de bloque en RAM comprimido.
- Permite swap rápido sin acceso a almacenamiento físico.

### Análisis de Memoria
- `/proc/meminfo`: información detallada del sistema.
- `smem`: consumo de memoria por proceso.
- `ps`, `top`: información en tiempo real.

---

## 6. Introducción a ZRAM en Dispositivos Embebidos

ZRAM es un módulo del kernel que permite crear dispositivos de bloque comprimidos en la RAM. Beneficios:
- Aumenta la memoria disponible de forma efectiva.
- Reduce accesos al almacenamiento, alargando su vida útil.
- Mejora el rendimiento de sistemas con poca RAM.

### Ejemplo de Configuración:
```bash
modprobe zram
echo lz4 > /sys/block/zram0/comp_algorithm
echo 64M > /sys/block/zram0/disksize
mkswap /dev/zram0
swapon /dev/zram0
```

---

## 7. Configuración y Uso de SWAP en Entornos Embebidos

Aunque tradicionalmente se evita el uso de swap en sistemas embebidos, hoy en día se puede considerar bajo ciertas condiciones:
- Uso de almacenamiento fiable (eMMC, NAND).
- Implementación de zswap o zram para compresión.

### Creación de un Archivo Swap:
```bash
dd if=/dev/zero of=/swapfile bs=1M count=128
chmod 600 /swapfile
mkswap /swapfile
swapon /swapfile
```

El sistema puede priorizar entre múltiples dispositivos swap con `swapon -p`.

---

## 8. Asignación de Memoria con `malloc()`, `free()`, `mmap()`

### `malloc()` y `free()`
- Asignan y liberan memoria dinámicamente en el *heap* del proceso.
- Necesarios para estructuras de datos de tamaño variable.

### `mmap()`
- Permite mapear archivos o dispositivos a memoria.
- Muy útil para trabajar con grandes cantidades de datos sin copiarlos.

### Ejemplos en C:
```c
void* buffer = malloc(1024);
if (!buffer) perror("malloc");
free(buffer);
```

```c
int fd = open("archivo.bin", O_RDONLY);
void* map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
munmap(map, size);
```

---

## 9. Depuración de Problemas de Memoria

### Valgrind
Detecta:
- Fugas de memoria.
- Accesos fuera de límites.
- Uso de memoria sin inicializar.

```bash
valgrind --leak-check=full ./app
```

### strace
- Muestra llamadas al sistema.
- Ayuda a identificar errores de asignación o acceso.

```bash
strace ./app
```

### gdb
- Depurador interactivo.
- Permite examinar memoria, registros, stack, etc.

```bash
gdb ./app
(gdb) run
```

---

## 10. Casos de Estudio

### Caso 1: Router doméstico con OpenWRT
- RAM limitada (32MB o 64MB).
- Uso de ZRAM para SWAP.
- Kernel personalizado.
- Binarios con `strip`.
- BusyBox para comandos.

### Caso 2: Dispositivo IoT con 64MB de RAM
- uClibc o musl como libc.
- Eliminación de demonios innecesarios.
- Kernel optimizado.
- Supervisión con `smem` y `top`.

### Caso 3: Sistema Automotriz IVI
- Interfaz gráfica intensiva.
- Memoria compartida entre procesos para buffers de video.
- Control de fugas crítico.
- Uso de `mmap()` para acceso eficiente a archivos multimedia.

