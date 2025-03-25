# Tema 11: Control de memoria

## Introducción a la gestión de memoria en Linux
Linux implementa un sistema de gestión de memoria altamente sofisticado, diseñado para funcionar de forma eficiente tanto en servidores de alto rendimiento como en sistemas embebidos con recursos limitados. Sus mecanismos incluyen memoria virtual, paginación, asignación dinámica y uso de swap, permitiendo una abstracción uniforme del hardware subyacente.

## Diferencia entre memoria física y virtual
- **Memoria física**: corresponde a los chips de RAM presentes en el sistema.
- **Memoria virtual**: es un espacio de direcciones abstracto proporcionado a cada proceso, que puede exceder el tamaño de la memoria física disponible.

Linux traduce direcciones virtuales a físicas mediante tablas de páginas mantenidas por la Unidad de Gestión de Memoria (MMU).

## Espacios de usuario y kernel
Linux separa dos espacios de memoria:
- **Espacio de usuario**: donde corren las aplicaciones. Tiene acceso restringido para evitar interferencia con el sistema.
- **Espacio kernel**: tiene acceso total al hardware. Alojado en una región protegida de la memoria virtual.

## Uso de la memoria en sistemas embebidos
Los sistemas embebidos suelen operar con recursos limitados. Por ello:
- El tamaño del kernel y de los binarios debe optimizarse.
- Se prefiere el uso de bibliotecas ligeras como uClibc o musl.
- Se eliminan servicios innecesarios y se controlan estrictamente las fugas de memoria.

## Técnicas de optimización de memoria en Linux embebido
- **Uso de `strip`**: elimina símbolos de depuración.
- **Librerías compartidas**: reducen el uso de RAM.
- **Configuración personalizada del kernel**: solo se habilitan los controladores necesarios.
- **ZRAM y SWAP comprimido**.
- **Revisar el mapa de memoria (`/proc/meminfo`) y estadísticas por proceso (`smem`)**.

## Introducción a ZRAM y su uso en dispositivos embebidos
**ZRAM** crea dispositivos de bloque comprimidos en RAM. Beneficios:
- Mejora el rendimiento al reducir accesos a almacenamiento lento.
- Aumenta la cantidad efectiva de memoria disponible.

### Ejemplo de configuración:
```bash
modprobe zram
echo lz4 > /sys/block/zram0/comp_algorithm
echo 64M > /sys/block/zram0/disksize
mkswap /dev/zram0
swapon /dev/zram0
```

## Configuración y uso de SWAP en entornos embebidos
Aunque tradicionalmente evitado, el uso de swap es posible y útil si:
- Se usa almacenamiento confiable (eMMC, NAND).
- Se implementa compresión (zswap o zram).

### Creación de archivo swap:
```bash
dd if=/dev/zero of=/swapfile bs=1M count=128
chmod 600 /swapfile
mkswap /swapfile
swapon /swapfile
```

## Asignación de memoria con `mmap()`, `malloc()`, `free()`
- `malloc()` y `free()` permiten asignar y liberar bloques dinámicamente desde el heap.
- `mmap()` permite mapear archivos o dispositivos directamente a memoria.

### Ejemplo:
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

## Depuración de problemas de memoria con `valgrind`, `strace`, `gdb`
- **Valgrind**: detecta fugas, accesos fuera de límites, doble liberación, etc.
  ```bash
  valgrind --leak-check=full ./app
  ```
- **strace**: registra llamadas al sistema.
  ```bash
  strace ./app
  ```
- **gdb**: depurador a nivel de código fuente.
  ```bash
  gdb ./app
  (gdb) run
  ```

## Casos de estudio de gestión de memoria en sistemas embebidos
1. **Router doméstico con OpenWRT**:
   - Uso extensivo de ZRAM.
   - SWAP comprimido para mantener servicios en RAM.

2. **Dispositivo IoT con Linux y 64MB RAM**:
   - Eliminación de servicios innecesarios.
   - BusyBox como shell y herramientas.
   - Kernel con configuración minimalista.

3. **Sistemas automotrices (IVI)**:
   - Uso de memoria compartida entre procesos para gráficos.
   - Monitorización constante de fugas.


