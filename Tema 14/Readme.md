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
- CPU: carga, uso por procesos.
- I/O: acceso a disco/red.
- Memoria: swaps, fugas, buffers.
- Sistema: latencias del kernel, interrupciones.

---

## 2. Uso de `top`, `htop`, `iotop` y `vmstat` para monitoreo

### `top`

Muestra en tiempo real los procesos activos y su uso de CPU y memoria.
```bash
top
```

### `htop`

Versión interactiva y visual de `top`, útil para identificar procesos problemáticos.
```bash
htop
```

### `iotop`

Muestra procesos que generan I/O en disco.
```bash
iotop
```

### `vmstat`

Estadísticas de memoria, procesos, I/O y CPU.
```bash
vmstat 1
```

**Uso típico:** iniciar estas herramientas en segundo plano durante pruebas funcionales para detectar anomalías.

---

## 3. Introducción a `perf` y su uso en profiling

`perf` es una herramienta del kernel de Linux para analizar el comportamiento de programas.

### Ejemplos de uso:
```bash
perf stat ./mi_aplicacion
```
Muestra estadísticas generales (instrucciones, ciclos, fallos de caché).

```bash
perf record ./mi_aplicacion
perf report
```
Captura y muestra un perfil de funciones ejecutadas, útil para identificar puntos calientes.

**Ventaja:** Bajo overhead, análisis detallado a nivel de instrucciones y símbolos.

---

## 4. Uso de `ftrace` y `trace-cmd` para tracing del kernel

### `ftrace`
Herramienta nativa del kernel para trazar llamadas internas.

Activación manual:
```bash
echo function > /sys/kernel/debug/tracing/current_tracer
cat /sys/kernel/debug/tracing/trace
```

### `trace-cmd`
Herramienta avanzada para capturar trazas de `ftrace`.
```bash
trace-cmd record -p function
trace-cmd report
```

**Aplicaciones:**
- Análisis de interrupciones.
- Diagnóstico de problemas en drivers.
- Seguimiento de latencias del scheduler.

---

## 5. Medición de latencias con herramientas de tracing (`latencytop`)

`latencytop` permite visualizar dónde se producen latencias en el sistema.

### Instalación:
```bash
apt install latencytop
```

### Ejecución:
```bash
latencytop
```

**Muestra:**
- Latencias por syscall.
- Motivos de espera (locks, I/O, swaps).

**Relevancia en sistemas embebidos:** Permite optimizar el comportamiento bajo condiciones de carga o tiempo real.

---

## 6. Análisis de cuellos de botella con `sysstat`

El paquete `sysstat` incluye herramientas como:

- `iostat`: estadísticas de dispositivos de almacenamiento.
- `mpstat`: uso de CPU por núcleo.
- `pidstat`: consumo por proceso.

### Ejemplo:
```bash
iostat -x 1
pidstat -r -u -d 1
```

**Aplicaciones:**
- Detección de procesos con alto uso de disco o CPU.
- Identificación de esperas excesivas de I/O.

---

## 7. Uso de `strace` para analizar llamadas al sistema

`strace` permite interceptar y registrar llamadas al sistema realizadas por un programa.

### Ejemplo básico:
```bash
strace ./mi_aplicacion
```

### Filtrar por tipo:
```bash
strace -e open,read,write ./mi_aplicacion
```

**Aplicaciones:**
- Diagnóstico de errores de ejecución.
- Análisis de accesos a archivos o sockets.
- Medición de tiempos por syscall (`-T`).

---

## 8. Técnicas avanzadas de profiling con `LTTng`

**LTTng (Linux Trace Toolkit Next Generation)** permite tracing de bajo overhead con granularidad a nivel de eventos del kernel y espacio de usuario.

### Ejemplo:
```bash
lttng create mi-traza
lttng enable-event -a -k
lttng start
# ejecutar aplicación
lttng stop
lttng view
```

**Ventajas:**
- Integración con Babeltrace.
- Trazas detalladas, útiles para tiempos de respuesta o análisis en sistemas de producción.

---

## 9. Optimización de rendimiento basada en análisis de profiling

El profiling no solo sirve para diagnosticar problemas, sino también para optimizar:

- **Reducir carga de CPU:** evitar polling, optimizar algoritmos.
- **Reducir I/O:** agrupar escrituras, usar buffers.
- **Minimizar latencias:** evitar bloqueos, reordenar prioridades.
- **Optimizar uso de memoria:** identificar fugas, ajustar tamaños de pila/heap.

**Ejemplo:** tras observar en `perf` que una función de decodificación consume el 50% del tiempo, se reescribe usando una librería optimizada con SIMD.

---

## 10. Casos de estudio en profiling y tracing en sistemas embebidos

### Caso 1: Gateway IoT lento al iniciar
- Se usa `bootchart` y `perf` para determinar que `systemd` espera un servicio innecesario.
- Se deshabilita dicho servicio, reduciendo el tiempo de arranque en 2 segundos.

### Caso 2: Dispositivo con bloqueos intermitentes
- `ftrace` revela que un driver mal implementado mantiene deshabilitadas las interrupciones.
- Se corrige el driver, eliminando las pausas.

### Caso 3: Uso excesivo de CPU
- `htop` y `perf` detectan que una rutina de chequeo de red usa `sleep` en vez de `select`.
- Se refactoriza, reduciendo uso de CPU del 40% al 3%.



