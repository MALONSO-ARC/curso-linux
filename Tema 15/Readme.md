# Tema 15: Linux en Tiempo Real (Real-Time Linux)

En muchas aplicaciones embebidas, no basta con que el sistema "funcione bien"; debe responder dentro de ciertos tiempos garantizados. Es aquí donde entra en juego el concepto de **tiempo real** en sistemas operativos. Este tema profundiza en cómo adaptar Linux para cumplir con requisitos de tiempo real.

---

## 1. Introducción a los sistemas en tiempo real

Un **sistema en tiempo real** es aquel que responde a eventos o estímulos del entorno dentro de un tiempo predecible y limitado. No se trata de ser el más rápido, sino de **cumplir siempre con los tiempos establecidos**.

### Tipos:
- **Tiempo real estricto (hard real-time):** fallar en el tiempo de respuesta puede tener consecuencias catastróficas (p. ej., frenos ABS).
- **Tiempo real suave (soft real-time):** los retardos no son deseables, pero no provocan fallo total (p. ej., video en streaming).

---

## 2. Diferencias entre RTOS y GPOS

| Característica         | RTOS                        | GPOS (Linux estándar)         |
|------------------------|-----------------------------|-------------------------------|
| Determinismo           | Alto                        | Bajo                          |
| Tiempo de respuesta    | Garantizado y predecible    | Variable                      |
| Overhead               | Mínimo                      | Moderado/alto                 |
| Soporte de características avanzadas | Limitado      | Amplio                        |
| Ejemplos               | FreeRTOS, Zephyr            | Ubuntu, Debian, Fedora        |

**Linux se comporta como un GPOS**, pero puede adaptarse con parches y configuraciones para acercarse al comportamiento de un RTOS.

---

## 3. Conceptos clave: latencia, determinismo y tiempo de respuesta

- **Latencia:** tiempo entre que ocurre un evento (ej. interrupción) y que el sistema comienza a responder.
- **Determinismo:** capacidad del sistema de garantizar tiempos de ejecución constantes ante las mismas condiciones.
- **Jitter:** variabilidad en el tiempo de respuesta.

**Ejemplo:** si un sistema tarda 2 ms en responder a una señal externa, pero a veces tarda 10 ms, no es determinista.

---

## 4. Introducción a PREEMPT-RT y su integración con Linux embebido

**PREEMPT-RT** es un conjunto de parches que transforma al kernel de Linux en un sistema más determinista y apto para tiempo real.

### Características:
- Permite la preempción total del kernel.
- Convierte partes del kernel en hilos (ej. controladores IRQ).
- Mejora la latencia al reducir las secciones no preemptibles.

### Integración:
- Disponible como parche o en algunas distribuciones (ej. Debian RT, Yocto con meta-rt).
- Compatible con herramientas estándar como `chrt`, `cyclictest`, `trace-cmd`.

---

## 5. Configuración del Kernel para tiempo real

### Requisitos:
- Descargar fuente del kernel compatible con PREEMPT-RT.
- Aplicar el parche RT (`patch-*.rt.patch`).
- Configurar con opciones:

```bash
make menuconfig
```

Activar:
- `CONFIG_PREEMPT_RT_FULL`
- `CONFIG_HIGH_RES_TIMERS`
- `CONFIG_IRQ_FORCED_THREADING`

### Compilación:
```bash
make -j$(nproc)
make modules_install
make install
```

**Ejemplo de prueba:**
```bash
cyclictest -t1 -p80 -n -i 1000 -l 1000000
```

---

## 6. Uso de Xenomai como alternativa a PREEMPT-RT

**Xenomai** es un framework que proporciona un entorno de ejecución en tiempo real _coexistiendo_ con Linux estándar.

### Modo de funcionamiento:
- Usa un microkernel en paralelo (modo co-kernel).
- Redirige llamadas del espacio de usuario a través de una capa de compatibilidad.

### Ventajas:
- Muy baja latencia (menor que PREEMPT-RT).
- API compatible con POSIX, VxWorks y otros RTOS.
- Buen soporte para aplicaciones industriales.

### Desventajas:
- Más complejo de integrar.
- Requiere toolchain y kernel específicos.

---

## 7. Comparación de PREEMPT-RT vs. Xenomai

| Característica          | PREEMPT-RT             | Xenomai                   |
|--------------------------|------------------------|---------------------------|
| Latencia típica          | 50-100 µs              | <20 µs                    |
| Integración              | Directa en kernel      | Kernel modificado + co-kernel |
| Complejidad              | Moderada               | Alta                      |
| Ecosistema               | Kernel principal       | Comunidad específica      |
| Compatibilidad           | Total con herramientas estándar | Requiere APIs específicas |

**Elección:** depende de la aplicación. Para tareas industriales con requisitos muy estrictos, Xenomai es preferido; para aplicaciones generalistas en Linux, PREEMPT-RT es más fácil de adoptar.

---

## 8. Análisis de latencias y técnicas de optimización

### Medición:
- `cyclictest`: mide jitter y latencias en respuesta a interrupciones temporizadas.
- `trace-cmd`, `ftrace`: traza eventos de scheduling y respuesta.

### Optimización:
- Asignar prioridad fija con `chrt`:
```bash
chrt -f 90 ./mi_tarea_rt
```
- Aislar CPUs con `isolcpus`, `nohz_full` en el kernel.
- Deshabilitar C-states o Turbo Boost.
- Afinar afinidad de CPU (`taskset`).

---

## 9. Programación de tareas en tiempo real en Linux embebido

En sistemas POSIX se puede usar `pthread` con atributos de planificación:

```c
struct sched_param param;
pthread_t hilo;
param.sched_priority = 80;
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
pthread_attr_setschedparam(&attr, &param);
pthread_create(&hilo, &attr, mi_funcion, NULL);
```

### Bibliotecas útiles:
- `rt` (realtime extensions)
- `libxenomai` para tareas en Xenomai

---

## 10. Casos de uso de Linux en tiempo real en la industria

- **Automoción:** ECU, control de frenos, suspensión activa.
- **Robótica:** control de motores, adquisición de sensores.
- **Audio profesional:** procesamiento en tiempo real, interfaces MIDI.
- **Automatización industrial:** PLCs, control de líneas de producción.

---

## 11. Consejos para seleccionar la mejor opción para proyectos real-time

- **¿Necesitas latencias <20 µs?** → Considera Xenomai.
- **¿Quieres usar herramientas estándar (systemd, bash, etc)?** → PREEMPT-RT.
- **¿Proyecto con soporte de comunidad y upstream?** → PREEMPT-RT.
- **¿Equipo con experiencia en RTOS?** → Xenomai puede resultar más natural.
- **¿Necesitas código certificable (seguridad funcional)?** → Evalúa RTOS dedicados + Linux como sistema paralelo.

**En resumen:**
- Si la prioridad es el rendimiento determinista puro → Xenomai.
- Si se busca equilibrio entre tiempo real y facilidad → PREEMPT-RT.


