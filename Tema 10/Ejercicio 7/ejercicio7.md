# Crear un demonio en Linux

**Objetivo:** Diseñar e implementar un programa en C que se ejecute como un **daemon** en segundo plano, registre su actividad en un archivo de log y maneje una señal de terminación.

---

## Enunciado

1. Crea un programa en C llamado `mi_daemon.c` que:
   - Se convierta en un proceso demonio.
   - Escriba un mensaje cada 3 segundos en `/tmp/daemon_ejercicio.log`.
   - Termine de forma limpia al recibir la señal `SIGTERM`.

2. Añade manejo de señales con `signal()` para capturar `SIGTERM` y realizar limpieza (como cerrar el archivo de log).

3. Compílalo, ejecútalo en background y comprueba que sigue activo.

---

## Código base sugerido
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

int log_fd;
int ejecutando = 1;

void manejar_senal(int sig) {
    if (sig == SIGTERM) {
        dprintf(log_fd, "Demonio recibió SIGTERM. Cerrando...\n");
        ejecutando = 0;
    }
}

int main() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    setsid();
    chdir("/");
    umask(0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGTERM, manejar_senal);

    log_fd = open("/tmp/daemon_ejercicio.log", O_WRONLY | O_CREAT | O_APPEND, 0644);

    while (ejecutando) {
        dprintf(log_fd, "[Demonio activo] PID: %d\n", getpid());
        sleep(3);
    }

    close(log_fd);
    return 0;
}
```

---

## Pasos para probarlo
1. Compila:
```bash
gcc mi_daemon.c -o mi_daemon
```

2. Ejecútalo:
```bash
./mi_daemon &
```

3. Verifica su funcionamiento:
```bash
tail -f /tmp/daemon_ejercicio.log
```

4. Detén el demonio:
```bash
kill -SIGTERM <PID>
```
(Obtén el PID con `ps` o desde el log)

---

## Preguntas de repaso
- ¿Qué pasaría si el demonio no maneja `SIGTERM`?
- ¿Por qué es necesario cerrar los descriptores estándar?
- ¿Qué beneficios ofrece setsid()?



