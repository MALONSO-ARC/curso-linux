# Tema 15: Linux en Tiempo Real (Real-Time Linux)

En muchas aplicaciones embebidas, no basta con que el sistema "funcione bien"; debe responder dentro de ciertos tiempos garantizados. Es aquí donde entra en juego el concepto de **tiempo real** en sistemas operativos. Este tema profundiza en cómo adaptar Linux para cumplir con requisitos de tiempo real, presentando conceptos clave, alternativas de kernel y herramientas de análisis.

---

## 1. Introducción a los sistemas en tiempo real

Un **sistema en tiempo real** es aquel que responde a eventos o estímulos del entorno dentro de un tiempo predecible y limitado. La clave no está en la velocidad, sino en la **previsibilidad**. 

### Tipos de sistemas en tiempo real:
- **Tiempo real estricto (hard real-time):**
  - Cualquier incumplimiento en el tiempo de respuesta puede tener consecuencias catastróficas.
  - Ejemplos: airbags, frenos ABS, marcapasos, controladores industriales críticos.

- **Tiempo real suave (soft real-time):**
  - Los fallos ocasionales no causan consecuencias graves, pero afectan la calidad.
  - Ejemplos: reproducción de audio/video, juegos interactivos, telecomunicaciones.

### ¿Por qué es relevante en sistemas embebidos?
Los sistemas embebidos suelen estar conectados al mundo físico (actuadores, sensores), donde los tiempos de reacción son cruciales para mantener la estabilidad y funcionalidad del sistema.

---

## 2. Diferencias entre RTOS y GPOS

| Característica         | RTOS (p.ej. FreeRTOS)       | GPOS (Linux estándar)         |
|------------------------|-----------------------------|-------------------------------|
| Determinismo           | Alto                        | Bajo                          |
| Tiempo de respuesta    | Garantizado y predecible    | Variable                      |
| Overhead               | Mínimo                      | Moderado/alto                 |
| Funcionalidades        | Limitadas y enfocadas       | Muy amplias                   |
| Aplicaciones típicas   | Control embebido, sensores  | Escritorio, servidores        |

Un **RTOS** está diseñado desde cero para cumplir tiempos de respuesta fijos, sacrificando algunas capacidades. Un **GPOS** como Linux ofrece muchas funcionalidades pero no prioriza el tiempo de respuesta, aunque puede adaptarse.

---

## 3. Conceptos clave: latencia, determinismo y tiempo de respuesta

### Latencia:
Tiempo desde que ocurre un evento (como una interrupción) hasta que el sistema comienza a procesarlo.

### Determinismo:
Capacidad del sistema para garantizar que los tiempos de respuesta sean constantes bajo las mismas condiciones.

### Jitter:
Variabilidad en los tiempos de respuesta. Un sistema con bajo jitter es más confiable para tareas críticas.

**Ejemplo práctico:**
Un sistema que generalmente responde en 2 ms pero ocasionalmente en 10 ms tiene un jitter elevado y es poco determinista.

---

## 4. Introducción a PREEMPT-RT y su integración con Linux embebido

**PREEMPT-RT** convierte al kernel de Linux en un sistema más determinista.

### Cambios clave introducidos por PREEMPT-RT:
- Se permite la **preempción completa del kernel**, incluyendo interrupciones.
- **Convertir IRQs en threads (Threaded IRQs)**: así pueden ser planificados con prioridades.
- Reducir zonas críticas no preemptibles (secciones protegidas con spinlocks).
- Soporte mejorado para temporizadores de alta resolución.

### Estado del proyecto:
- PREEMPT-RT ha sido **integrado completamente en el mainline del kernel** desde la versión **6.12**.
- Ya no se necesita aplicar parches externos para usarlo.

### Usos típicos:
- Sistemas industriales con Linux.
- Robótica, audio de baja latencia, automoción.

---

## 5. Configuración del Kernel para tiempo real

### Requisitos:
- Usar kernel Linux 6.12 o superior.
- Activar opciones de configuración RT durante la compilación.

### Proceso general con kernel 6.12 o superior:

Descargar y preparar el kernel:
```bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.12.tar.xz

# Desempaquetar
 tar -xf linux-6.12.tar.xz
cd linux-6.12
```

Configurar kernel:
```bash
make menuconfig
```
Activar:
- `CONFIG_PREEMPT_RT=y` → Seleccionar "Fully Preemptible Kernel (Real-Time)"
- `CONFIG_HIGH_RES_TIMERS=y`
- `CONFIG_IRQ_FORCED_THREADING=y`

Compilar e instalar:
```bash
make -j$(nproc)
sudo make modules_install
sudo make install
```

Actualizar GRUB y reiniciar:
```bash
sudo update-grub
sudo reboot
```

Verificar que se está usando el kernel RT:
```bash
uname -a
grep PREEMPT /boot/config-$(uname -r)
```

Prueba de latencias:
```bash
cyclictest -t1 -p80 -n -i 1000 -l 1000000
```

Un resultado con latencias consistentes por debajo de 100 µs es un buen indicador de un sistema RT bien configurado.

---

## 6. Uso de Xenomai como alternativa a PREEMPT-RT

**Xenomai** permite ejecutar tareas de tiempo real con latencias muy bajas y predecibles, al trabajar junto al kernel Linux mediante un enfoque de co-kernel.

### Arquitectura:
- Añade un **co-kernel** (Cobalt) que gestiona las tareas críticas de tiempo real.
- Linux opera como un sistema de fondo para tareas no críticas.
- Las aplicaciones en tiempo real utilizan APIs específicas para acceder al co-kernel.

### Proceso para instalar Xenomai (versión 3.x):

1. **Descargar el kernel y Xenomai:**
```bash
wget https://xenomai.org/downloads/ipipe/v5.x/x86/ipipe-core-5.4.106-x86-10.patch
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.4.106.tar.xz
wget https://xenomai.org/downloads/xenomai/stable/xenomai-3.1.tar.bz2
```

2. **Preparar el kernel:**
```bash
tar -xf linux-5.4.106.tar.xz
cd linux-5.4.106
patch -p1 < ../ipipe-core-5.4.106-x86-10.patch
```

3. **Configurar el kernel:**
```bash
make menuconfig
```
Activar:
- Soporte para IPIPE.
- Opciones necesarias para Cobalt.

4. **Compilar e instalar el kernel:**
```bash
make -j$(nproc)
sudo make modules_install
sudo make install
```

5. **Compilar Xenomai y preparar toolchain:**
```bash
tar -xf ../xenomai-3.1.tar.bz2
cd xenomai-3.1
./scripts/bootstrap
./configure --with-pic --with-core=cobalt --enable-smp
make -j$(nproc)
sudo make install
```

6. **Verificar instalación y configuración:**
```bash
/opt/xenomai/bin/xeno-config --skin=posix --cflags --ldflags
```

### Separación de aplicaciones RT y no-RT:
- Las aplicaciones que usan la API Xenomai (POSIX, Alchemy, RTDM) corren sobre el co-kernel.
- El resto de Linux (bash, systemd, servicios) se ejecuta sobre el kernel estándar, sin prioridad real-time.
- Se recomienda ejecutar las aplicaciones RT fijando la CPU:
```bash
taskset -c 1 ./mi_aplicacion_rt
```
- También se pueden usar opciones del kernel como `nohz_full` y `isolcpus` para aislar núcleos para RT:
```
grub: isolcpus=1 nohz_full=1 rcu_nocbs=1
```

### Casos de uso típicos:
- Robótica de precisión.
- Automatización industrial con requisitos de certificación.
- Control de motores y sincronización fina.

## 7. Comparación de PREEMPT-RT vs. Xenomai

| Característica          | PREEMPT-RT             | Xenomai                   |
|--------------------------|------------------------|---------------------------|
| Latencia típica          | 50-100 µs              | <20 µs                    |
| Integración              | En kernel principal    | Co-kernel + kernel mod    |
| Complejidad              | Baja a moderada        | Alta                      |
| Compatibilidad           | 100% con herramientas Linux | APIs específicas        |
| Adecuado para            | General, robótica, audio | Tiempos críticos estrictos |

---

## 8. Análisis de latencias y técnicas de optimización

### Herramientas:
- `cyclictest`: mide latencia y jitter.
- `trace-cmd`, `ftrace`: traza eventos como interrupciones, wakeups. [ftrace](https://www.kernel.org/doc/html/v5.1/trace/ftrace.html#:~:text=One%20of%20the%20most%20common,txt%20for%20more%20information.)
- `latencytop`: muestra causas comunes de latencia.

### Optimización práctica:
- Usar `chrt` o `schedtool` para fijar prioridades:
```bash
chrt -f 90 ./mi_aplicacion
```
- Afinar CPUs:
  - Aislar CPUs (`isolcpus=1` en GRUB).
  - Afinidad con `taskset`.
  - Desactivar Turbo Boost y C-states (en BIOS o `/sys`).

### Ejemplo:
```bash
sudo cyclictest -S -p 80 -i 1000 -n -l 100000
```

---

## 9. Programación de tareas en tiempo real en Linux embebido

Linux soporta programación en tiempo real mediante **POSIX Threads** con políticas `SCHED_FIFO` o `SCHED_RR`.

### Código ejemplo:
```c
#include <pthread.h>
#include <sched.h>

void* tarea_rt(void* arg) {
    while (1) { /* operación crítica */ }
}

int main() {
    struct sched_param param;
    pthread_attr_t attr;
    pthread_t hilo;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = 80;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&hilo, &attr, tarea_rt, NULL);
    pthread_join(hilo, NULL);
    return 0;
}
```

### Requiere:
- Ejecutar como root o tener capacidades elevadas (`CAP_SYS_NICE`).

---

## 10. Casos de uso de Linux en tiempo real en la industria

- **Automoción:** sistemas ADAS, controladores electrónicos, gateways CAN.
- **Robótica industrial:** manipulación precisa, bucles de control cerrados.
- **Música/audio profesional:** procesamiento con baja latencia, DAW.
- **Aeronáutica:** control de vuelo, grabación de sensores.
- **Médico:** sistemas de adquisición y análisis de señales vitales.

---

## 11. Consejos para seleccionar la mejor opción para proyectos real-time

**Preguntas clave para elegir PREEMPT-RT o Xenomai:**
- ¿Cuál es la latencia máxima aceptable?
- ¿Se necesita compatibilidad con herramientas estándar?
- ¿Hay personal con experiencia en RTOS?
- ¿Existen requisitos de certificación o auditoría?

### Recomendaciones:
- **<20 µs y aplicación crítica:** Xenomai.
- **Requiere integración con Linux convencional:** PREEMPT-RT.
- **Portabilidad y facilidad:** PREEMPT-RT.
- **Sistema mixto (tiempo real + red, GUI, almacenamiento):** PREEMPT-RT con CPUs dedicadas.

---

## Recursos adicionales

- [PREEMPT-RT wiki](https://wiki.linuxfoundation.org/realtime)
- [Xenomai Project](https://xenomai.org/)
- [Real-Time Linux Analysis Tools](https://wiki.linuxfoundation.org/realtime/documentation/howto/tools)
- "Linux for Embedded and Real-Time Applications" - Doug Abbott
- "Building Embedded Linux Systems" - Karim Yaghmour

