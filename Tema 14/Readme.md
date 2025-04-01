# Tema 14: Profiling y Tracing en Linux Embebido

La optimización del rendimiento en sistemas embebidos es crucial debido a sus recursos limitados y requerimientos de tiempo real. El uso adecuado de herramientas de **profiling** (análisis de rendimiento) y **tracing** (seguimiento de eventos) permite identificar cuellos de botella, medir latencias y mejorar la eficiencia del sistema.

---

## 1. Introducción al análisis de rendimiento en sistemas embebidos

El análisis de rendimiento busca responder preguntas como:
- ¿Qué procesos consumen más CPU?
- ¿Dónde se generan latencias?
- ¿Qué llamadas al sistema ralentizan la ejecución?

El desafío en sistemas embebidos es realizar este análisis con herramientas ligeras, sin afectar demasiado al comportamiento del sistema.

**Ámbitos del análisis:**
- **CPU:** carga, uso por procesos, interrupciones.
- **I/O:** acceso a disco/red, cuellos de botella.
- **Memoria:** uso, swaps, fugas, buffers.
- **Sistema:** latencias del kernel, syscall lentas.

Ejemplo de comandos base:
```bash
top -n 1           # Uso de CPU y memoria
vmstat 1 5         # Estadísticas de memoria y CPU
```

---

## 2. Uso de `top`, `htop`, `iotop` y `vmstat` para monitoreo

### `top`
Muestra en tiempo real los procesos activos y su uso de CPU y memoria.
```bash
top
```

► Usa teclas como `P` para ordenar por uso de CPU o `M` por uso de memoria.

### `htop`
Versión interactiva y visual de `top`, permite navegar con cursores.
```bash
htop
```

► Muestra árbol de procesos, permite matar procesos, ver prioridades, etc.

### `iotop`
Muestra procesos que generan mayor I/O en disco (requiere permisos root).
```bash
sudo iotop
```

► Permite detectar procesos que escriben o leen en exceso.

### `vmstat`
Muestra estadísticas de procesos, memoria, paginación, I/O, CPU.
```bash
vmstat 1 10
```

► Indica bloqueos, swaps, interrupciones por segundo, etc.

**Consejo:** lanzar estas herramientas en segundo plano durante pruebas automatizadas:
```bash
htop -d 10 > log_htop.txt &
```

---

## 3. Introducción a `perf` y su uso en profiling

`perf` es una herramienta del kernel de Linux para recolectar información detallada del rendimiento.

### Comandos básicos:
```bash
perf stat ./mi_aplicacion
```
Muestra: instrucciones ejecutadas, ciclos, fallos de caché, etc.

```bash
perf record ./mi_aplicacion
perf report
```
Captura un perfil con las funciones más costosas (hotspots).

### Ejemplo de aplicación:
```c
#include <stdio.h>
int main() {
    for (volatile int i = 0; i < 100000000; i++);
    printf("Fin del bucle\n");
    return 0;
}
```
Compilar con información de depuración:
```bash
gcc -g -O2 ejemplo_perf.c -o ejemplo_perf
perf record ./ejemplo_perf
perf report
```

### Análisis gráfico adicional:
```bash
perf script > out.perf
```

---

## 4. Uso de `ftrace` y `trace-cmd` para tracing del kernel

### `ftrace`
Herramienta nativa para trazar funciones internas del kernel.

Activación manual:
```bash
echo function > /sys/kernel/debug/tracing/current_tracer
cat /sys/kernel/debug/tracing/trace
```

Filtrar funciones específicas:
```bash
echo mi_funcion > /sys/kernel/debug/tracing/set_ftrace_filter
```

Desactivación:
```bash
echo nop > /sys/kernel/debug/tracing/current_tracer
```

### `trace-cmd`
Interfaz para capturar trazas y analizarlas posteriormente.
```bash
trace-cmd record -p function
trace-cmd report
```

► Permite guardar trazas de eventos de forma más organizada.

Aplicaciones típicas:
- Detección de funciones que generan latencia.
- Análisis de drivers.
- Scheduling y preemption.

---

## 5. Medición de latencias con `latencytop`

Visualiza las fuentes de latencia más relevantes en tiempo real.

### Instalación:
```bash
sudo apt install latencytop
```

### Ejecución:
```bash
sudo latencytop
```

### Información que muestra:
- Tiempo de espera en syscall.
- Causa de la espera (mutex, disco, etc).

Ideal para sistemas con requisitos de tiempo real.

---

## 6. Análisis de cuellos de botella con `sysstat`

Incluye herramientas como:
- `iostat`: rendimiento de discos.
- `mpstat`: uso de CPU por núcleo.
- `pidstat`: uso por proceso.

### Ejemplos de uso:
```bash
iostat -x 1              # Por dispositivo
mpstat -P ALL 1         # CPU por núcleo
pidstat -r -u -d 1      # RAM, CPU, disco por PID
```

► Muy útil para detección de saturaciones por hardware o proceso.

---

## 7. Uso de `strace` para analizar llamadas al sistema

Intercepta e imprime todas las llamadas al sistema que realiza un proceso.

### Ejemplo básico:
```bash
strace ./mi_aplicacion
```

### Con filtros y tiempos:
```bash
strace -e trace=open,read,write -T ./mi_aplicacion
```

### Para depurar fallos:
```bash
strace -f -o salida.txt ./mi_aplicacion
```

Aplicaciones comunes:
- Ver archivos abiertos.
- Llamadas que fallan ("ENOENT").
- Medir tiempo por syscall.

---

## 8. Técnicas avanzadas de profiling con `LTTng`

**LTTng** es una suite de tracing para kernel y espacio de usuario.

### Flujo típico:
```bash
lttng create mi_sesion
lttng enable-event -a -k     # Eventos del kernel
lttng start
./mi_aplicacion
lttng stop
lttng view
```

### Uso de `babeltrace`:
```bash
babeltrace ~/lttng-traces/mi_sesion-*/
```

Ventajas:
- Bajo overhead.
- Precisión temporal.
- Soporte gráfico con herramientas como Trace Compass.

Ideal para:
- Sistemas en producción.
- Análisis post-mortem.

---

## 9. Optimización de rendimiento basada en análisis de profiling

### Acciones comunes:

- **CPU:**
  - Reemplazar bucles con `select()` o `poll()`.
  - Usar timers eficientes.

- **I/O:**
  - Agrupar escrituras.
  - Evitar acceso frecuente a disco.

- **Latencias:**
  - Reducir locks innecesarios.
  - Priorizar tareas críticas (RT).

- **Memoria:**
  - Usar `valgrind`, `massif`, `smem`.
  - Revisar configuración de `malloc`, `stack size`.

### Ejemplo de código:
```c
// Ineficiente
while (1) {
    if (check_socket()) procesar();
    usleep(1000);
}

// Mejorado
fd_set readfds;
select(sock+1, &readfds, NULL, NULL, NULL);
```

---

## 10. Casos de estudio en profiling y tracing en sistemas embebidos

### Caso 1: Gateway IoT lento al iniciar
- Herramientas: `bootchart`, `perf`.
- Problema: `systemd` espera servicio innecesario.
- Solución: eliminar dependencia, arranque de 9s a 6.5s.

### Caso 2: Bloqueos intermitentes en dispositivo
- Herramientas: `ftrace`, `trace-cmd`.
- Problema: driver mal implementado bloqueaba IRQs.
- Solución: corrección de driver, desaparecen pausas.

### Caso 3: CPU al 40% constantemente
- Herramientas: `htop`, `perf`.
- Problema: rutina de red usaba polling.
- Solución: cambio a `select()`, uso cae al 3%.

---

## Recursos adicionales

- [LTTng Documentation](https://lttng.org/docs)
- [perf wiki](https://perf.wiki.kernel.org)
- [Linux Tracing Toolkit](https://lttng.org/)
- [Brendan Gregg - Linux Performance](http://www.brendangregg.com/linuxperf.html)
- Libro: "Mastering Embedded Linux Programming"

