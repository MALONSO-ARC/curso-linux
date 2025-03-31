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

**Consejo:** iniciar estas herramientas en segundo plano durante pruebas funcionales para detectar anomalías.

---

## 3. Introducción a `perf` y su uso en profiling

`perf` es una herramienta del kernel de Linux para analizar el comportamiento de programas.

### Ejemplos de uso básico:
```bash
perf stat ./mi_aplicacion
```
Muestra estadísticas generales: instrucciones ejecutadas, ciclos de CPU, fallos de caché.

```bash
perf record ./mi_aplicacion
perf report
```
Captura un perfil de funciones ejecutadas y muestra un informe visual del consumo por función.

### Ejemplo práctico:
```c
// ejemplo_perf.c
#include <stdio.h>
int main() {
    for (volatile int i = 0; i < 100000000; i++);
    printf("Fin del bucle\n");
    return 0;
}
```
Compilar con símbolos:
```bash
gcc -g -O2 ejemplo_perf.c -o ejemplo_perf
perf record ./ejemplo_perf
perf report
```

---

## 4. Uso de `ftrace` y `trace-cmd` para tracing del kernel

### `ftrace`
Herramienta nativa del kernel para trazar llamadas internas.

Activación manual:
```bash
echo function > /sys/kernel/debug/tracing/current_tracer
cat /sys/kernel/debug/tracing/trace
```
Filtrar funciones:
```bash
echo 'mi_funcion' > /sys/kernel/debug/tracing/set_ftrace_filter
```

### `trace-cmd`
Herramienta avanzada para capturar y analizar trazas de `ftrace`.
```bash
trace-cmd record -p function
trace-cmd report
```

**Aplicaciones comunes:**
- Diagnóstico de latencias e interrupciones.
- Análisis de drivers y scheduling.

---

## 5. Medición de latencias con `latencytop`

`latencytop` permite visualizar dónde se producen latencias en el sistema.

### Instalación:
```bash
sudo apt install latencytop
```

### Ejecución:
```bash
sudo latencytop
```

**Muestra:**
- Latencias por syscall.
- Motivos de espera: locks, acceso a disco, swaps.

**Importancia:** Especialmente útil en sistemas con requerimientos de tiempo real o baja latencia.

---

## 6. Análisis de cuellos de botella con `sysstat`

El paquete `sysstat` incluye:
- `iostat`: estadísticas de dispositivos de almacenamiento.
- `mpstat`: uso de CPU por núcleo.
- `pidstat`: consumo por proceso.

### Uso práctico:
```bash
iostat -x 1
mpstat -P ALL 1
pidstat -r -u -d 1
```

**Consejo:** Ejecutar durante pruebas intensivas para detectar procesos o recursos saturados.

---

## 7. Uso de `strace` para analizar llamadas al sistema

`strace` intercepta llamadas al sistema y señales generadas por un programa.

### Ejemplo básico:
```bash
strace ./mi_aplicacion
```

### Filtrado y tiempo:
```bash
strace -e trace=open,read,write -T ./mi_aplicacion
```

**Aplicaciones:**
- Diagnóstico de fallos (archivos no encontrados, permisos).
- Análisis de rendimiento (tiempo por syscall).

---

## 8. Técnicas avanzadas de profiling con `LTTng`

**LTTng** es una solución avanzada para tracing de espacio de usuario y kernel.

### Pasos básicos:
```bash
lttng create sesion
lttng enable-event -a -k
lttng start
./mi_aplicacion
lttng stop
lttng view
```

### Integración con Babeltrace:
Permite convertir trazas a formato legible o exportarlas para análisis gráfico.

**Ventajas:**
- Bajo overhead.
- Detalles temporales precisos.

**Ideal para:**
- Sistemas con carga constante.
- Análisis profundo de interacción kernel/usuario.

---

## 9. Optimización de rendimiento basada en análisis de profiling

Una vez identificados los cuellos de botella, se pueden aplicar optimizaciones:

- **Reducir carga de CPU:**
  - Evitar bucles de polling.
  - Usar interrupciones o eventos.

- **Reducir I/O:**
  - Agrupar escrituras.
  - Evitar accesos innecesarios a disco.

- **Minimizar latencias:**
  - Evitar bloqueos por mutex.
  - Usar prioridades adecuadas (real-time).

- **Optimizar memoria:**
  - Identificar fugas con Valgrind.
  - Ajustar pilas en hilos o buffers.

### Ejemplo:
```c
// Uso ineficiente
while (1) {
  if (check_socket()) procesar();
  usleep(1000);
}

// Optimización
select(...);
```

---

## 10. Casos de estudio en profiling y tracing en sistemas embebidos

### Caso 1: Gateway IoT lento al iniciar
- Se usa `bootchart` y `perf`.
- Se detecta que `systemd` espera un servicio innecesario.
- Al eliminarlo, el arranque se reduce de 9s a 6.5s.

### Caso 2: Dispositivo con bloqueos intermitentes
- `ftrace` revela que un driver mal implementado bloquea interrupciones.
- Al corregirlo, las pausas desaparecen.

### Caso 3: Uso excesivo de CPU
- `htop` y `perf` detectan que una rutina de red hace polling.
- Se cambia a `select()` y el uso de CPU baja del 40% al 3%.

---

## Recursos adicionales

- [LTTng Documentation](https://lttng.org/docs)
- [perf wiki](https://perf.wiki.kernel.org)
- [Linux Tracing Toolkit](https://lttng.org/)
- "Linux Performance" - Brendan Gregg
- "Mastering Embedded Linux Programming"




