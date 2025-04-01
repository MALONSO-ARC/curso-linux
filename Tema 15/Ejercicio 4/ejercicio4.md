# Ejercicio 4: Compilar y ejecutar una tarea POSIX con prioridad RT

**Objetivo:** Crear una tarea usando `pthread` con política `SCHED_FIFO`.

## Código:
```c
// rt_task.c
#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>

void* tarea(void* arg) {
    while (1) {
        printf("Tarea en tiempo real ejecutándose...\n");
        usleep(500000);
    }
    return NULL;
}

int main() {
    pthread_t hilo;
    struct sched_param param;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = 80;
    pthread_attr_setschedparam(&attr, &param);

    pthread_create(&hilo, &attr, tarea, NULL);
    pthread_join(hilo, NULL);
    return 0;
}
```
### Compilación:
```bash
gcc -o rt_task rt_task.c -lpthread
sudo ./rt_task
```

**Preguntas:**
- ¿Qué prioridad tiene el hilo?
- ¿Qué pasa si compilas sin usar `sudo`?