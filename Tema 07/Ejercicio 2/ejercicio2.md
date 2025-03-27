# Análisis del RootFS en la Máquina Virtual de Desarrollo

## Objetivo

Familiarizarse con el análisis de un sistema de archivos root (RootFS) real, utilizando las herramientas disponibles en la máquina virtual del curso. Se verificará el tipo de sistema de archivos, la configuración del kernel y el comportamiento de procesos en tiempo de ejecución.

---

## Pasos a realizar

### 1. Verificar el tipo de sistema de archivos montado como root

Usar el comando `mount` o `findmnt`:
```bash
mount | grep ' on / '
```
O bien:
```bash
findmnt /
```

### 2. Ver la línea de comandos del kernel (cmdline)

Leer el contenido de `/proc/cmdline`:
```bash
cat /proc/cmdline
```

Anotar:
- El tipo de root (`root=/dev/...`)
- Parámetros de consola
- Modo de arranque (`ro`, `rw`, `quiet`, etc.)

### 3. Ver el sistema de archivos montados

```bash
cat /etc/fstab
mount
```

### 4. Observar procesos y sus accesos con `lsof`

Instalar si no está disponible:
```bash
sudo apt install lsof
```

Ejecutar:
```bash
lsof +D /etc
```

Verifica qué procesos están accediendo a archivos de configuración.

### 5. Analizar ejecución de un binario con `strace`

Ejemplo:
```bash
strace ls
```

Interpretar:
- Llamadas a `open`, `read`, `write`, `execve`
- Ficheros que intenta cargar

Guardar el resultado en un archivo:
```bash
strace -o salida_ls.txt ls
```

### 6. Consultar información del sistema

```bash
uname -a
lsb_release -a
cat /etc/os-release
```

### 7. Revisar permisos del init del sistema

```bash
ls -l /sbin/init
file /sbin/init
```

---

## Preguntas de Reflexión

1. ¿Qué tipo de sistema de archivos está montado como root?
2. ¿El rootfs está montado en modo lectura/escritura o solo lectura?
3. ¿Qué ficheros se abren cuando se ejecuta un comando simple como `ls`?
4. ¿Cómo cambiarías los permisos de un archivo para que no pueda ser leído por otros usuarios?

---

## Extra (opcional)

- Buscar archivos con permisos inseguros:
```bash
find / -perm -o+w 2>/dev/null
```
- Inspeccionar procesos huérfanos o zombies:
```bash
ps aux | grep defunct
```

---

