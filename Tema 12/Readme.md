# Tema 12: Depuración en Linux Embebido

## Introducción a técnicas de depuración en Linux
La depuración en sistemas embebidos es esencial para garantizar la estabilidad y eficiencia de los dispositivos. A diferencia del entorno de escritorio, el espacio limitado, la ausencia de interfaz gráfica y el acceso restringido al hardware presentan retos adicionales.

Linux embebido ofrece herramientas potentes para depurar tanto espacio de usuario como el espacio del kernel. Las técnicas incluyen desde depuración clásica con GDB hasta el análisis de trazas, registros del sistema y uso de herramientas especializadas como `valgrind` o `ftrace`.

---

## Uso del depurador **GDB** en sistemas embebidos
**GDB** es el depurador por excelencia para programas en C/C++.

### Características clave:
- Puntos de interrupción (breakpoints)
- Inspección de variables y memoria
- Seguimiento paso a paso

### Ejemplo:
Compilar con información de depuración:
```bash
gcc -g -o app main.c
```

Usar GDB:
```bash
gdb ./app
(gdb) break main
(gdb) run
(gdb) print variable
(gdb) next
```

---

## Depuración remota con **gdbserver**
Ideal cuando el objetivo embebido no tiene GDB instalado.

### Pasos:
1. En el dispositivo:
```bash
gdbserver :1234 ./app
```
2. En la PC de desarrollo:
```bash
gdb ./app
(gdb) target remote IP_DEL_OBJETIVO:1234
```

Permite depurar remotamente desde la comodidad del entorno de desarrollo.

---

## Análisis de fallos con **Core Dump**
Los *core dumps* capturan el estado de un proceso cuando falla.

### Habilitar core dumps:
```bash
ulimit -c unlimited
```

### Inspección:
```bash
gdb ./app core
(gdb) bt  # Backtrace
```

Permite estudiar errores incluso sin acceso al dispositivo al momento del fallo.

---

## Logs y análisis con `dmesg` y `journalctl`
- `dmesg`: muestra mensajes del kernel (arranque, errores de drivers, etc.)
- `journalctl`: accede a logs persistentes de `systemd`

### Ejemplos:
```bash
dmesg | grep error
journalctl -u nombre_servicio
```

Muy útil para detectar fallos de hardware, errores de drivers o reinicios inesperados.

---

## Monitorización en tiempo real con `top`, `htop`, `iotop`
- `top`: procesos en ejecución y uso de CPU/RAM
- `htop`: versión mejorada de `top`
- `iotop`: uso de E/S de disco por proceso

### Ejemplo:
```bash
htop
```

Ayudan a detectar cuellos de botella, procesos zombis o leaks de memoria.

---

## Depuración de drivers y módulos del Kernel
- Uso de macros como `pr_info()`, `pr_err()`, `dev_dbg()`.
- Activar `CONFIG_DYNAMIC_DEBUG` para habilitar debug selectivo.

### Revisar mensajes:
```bash
dmesg | tail -n 50
```

### Insertar módulo con depuración:
```bash
modprobe mi_modulo debug=1
```

### Acceso a trazas:
```bash
echo "file mi_driver.c +p" > /sys/kernel/debug/dynamic_debug/control
```

---

## Depuración de problemas de memoria con herramientas como **valgrind**
`valgrind` analiza el uso de memoria en programas de espacio de usuario.

### Uso:
```bash
valgrind --leak-check=full ./app
```

Detecta:
- Fugas de memoria
- Uso de memoria no inicializada
- Doble liberación de punteros

---

## Estrategias para depuración en hardware real
- Usar consola série para salida de logs.
- Mantener el kernel con símbolos (`vmlinux`).
- Emplear initramfs con herramientas de depuración.
- Utilizar GPIO/LEDs para debug visual.
- Probar con hardware simulado (QEMU) antes de probar en placa real.

### Ejemplo: Debug con LED GPIO
```c
gpio_set_value(DEBUG_LED, 1); // encender LED al entrar en función
```

---

## Ejercicios prácticos de depuración en entornos embebidos
1. **Crash en aplicación C**:
   - Recompilar con `-g` y usar GDB para localizar el fallo.
2. **Módulo de kernel no carga**:
   - Usar `dmesg` para ver por qué falla el `insmod`.
3. **Aplicación con fuga de memoria**:
   - Ejecutar con `valgrind` y encontrar el `malloc()` sin `free()`.
4. **Problema de rendimiento**:
   - Usar `iotop` o `htop` para detectar el proceso culpable.
5. **Depuración remota**:
   - Ejecutar `gdbserver` en el objetivo y conectar desde el host.

