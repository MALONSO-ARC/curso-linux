# Procesos e Hilos en Linux Embebido

## 1. Introducción a los procesos en Linux

En Linux, un **proceso** es una instancia en ejecución de un programa. Cada proceso tiene su propio espacio de memoria y recursos asignados por el sistema operativo. En sistemas embebidos, la gestión eficiente de procesos es fundamental para garantizar el rendimiento y la estabilidad del sistema.

### **Características clave de los procesos en Linux:**
- Cada proceso tiene un **PID (Process ID)** único.
- Se ejecutan en espacio de usuario o en espacio de kernel.
- Se pueden comunicar entre sí mediante **IPC (Inter-Process Communication)**.
- El sistema gestiona los procesos a través de un **scheduler**.

Podemos listar los procesos en ejecución con:
```bash
ps aux
```
O verlos en tiempo real con:
```bash
top
```

---

## 2. Creación y gestión de procesos con `fork()`

Para crear un nuevo proceso en Linux se utiliza la llamada al sistema `fork()`, que genera un proceso hijo duplicando el padre.

### **Ejemplo de `fork()`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        exit(1);
    } else if (pid == 0) {
        printf("Soy el proceso hijo con PID: %d\n", getpid());
    } else {
        printf("Soy el proceso padre con PID: %d y mi hijo tiene PID: %d\n", getpid(), pid);
    }
    return 0;
}
```
### **Gestión de procesos**
Podemos esperar a que un proceso hijo termine con `wait()` o `waitpid()`:
```c
#include <sys/types.h>
#include <sys/wait.h>

wait(NULL); // Espera a que cualquier hijo termine
```

---

## 3. Introducción a hilos (`pthread`)

Los **hilos (threads)** son unidades más ligeras de ejecución dentro de un proceso. Comparten la memoria del proceso y pueden ejecutarse en paralelo.

### **Ejemplo de `pthread`**
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *print_message(void *ptr) {
    printf("Hilo en ejecución: %ld\n", pthread_self());
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, print_message, NULL);
    pthread_join(thread, NULL);
    return 0;
}
```

---

## 4. Comunicación entre procesos: Pipes, FIFO, Shared Memory

La **comunicación entre procesos (IPC)** en Linux embebido es esencial cuando varios procesos necesitan intercambiar datos.

### **4.1 Pipes (Tubos anónimos)**
Los **pipes** permiten la comunicación entre procesos relacionados.
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    char buffer[20];
    if (fork() == 0) {
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Mensaje recibido en hijo: %s\n", buffer);
    } else {
        close(fd[0]);
        write(fd[1], "Hola desde padre", 17);
    }
    return 0;
}
```

### **4.2 FIFO (Pipes con nombre)**
Permiten comunicación entre procesos no relacionados.
```bash
mkfifo /tmp/mi_fifo
```
Lectura:
```bash
cat /tmp/mi_fifo
```
Escritura:
```bash
echo "Hola" > /tmp/mi_fifo
```

### **4.3 Memoria compartida (`shm`)**
```c
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHM_SIZE 1024

int main() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    char *shm = shmat(shmid, NULL, 0);
    sprintf(shm, "Mensaje en memoria compartida");
    printf("%s\n", shm);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
```

---

## 5. Sincronización de hilos: Mutex y Semáforos

Cuando múltiples hilos acceden a recursos compartidos, es necesario sincronizarlos.

### **5.1 Mutex**
```c
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
pthread_mutex_lock(&lock);
// Sección crítica
pthread_mutex_unlock(&lock);
pthread_mutex_destroy(&lock);
```

### **5.2 Semáforos**
```c
sem_t sem;
sem_init(&sem, 0, 1);
sem_wait(&sem);
// Sección crítica
sem_post(&sem);
sem_destroy(&sem);
```

---

## 6. Prioridades de procesos y scheduling en Linux

En sistemas embebidos, el **scheduler** determina el orden de ejecución de procesos.

### **Cambiar prioridad con `nice` y `renice`**
```bash
nice -n 10 ./mi_programa
renice -n -5 -p 1234
```

### **Ver prioridad de procesos**
```bash
top -o %PR
```

---

## 7. Ejecución en background y demonios

Ejecutar un proceso en **background**:
```bash
./mi_programa &
```

Ejemplo de un **demonio**:
```c
pid_t pid = fork();
if (pid > 0) exit(0);
setsid();
chdir("/");
umask(0);
while (1) {
    sleep(1);
}
```

---

## 8. Debugging de procesos y detección de bloqueos

### **Ver procesos bloqueados**
```bash
ps -eo pid,stat,cmd | grep ^[D]
```

### **Usar `strace`**
```bash
strace -p 1234
```

---

## 9. Medición de tiempos de ejecución en Linux embebido

### **`time` para medir ejecución de procesos**
```bash
time ./mi_programa
```

### **Usar `clock_gettime()` en C**
```c
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
// Código
clock_gettime(CLOCK_MONOTONIC, &end);
double tiempo = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
```

---

## 10. Ejemplo práctico: IPC en Linux embebido

Implementar IPC con memoria compartida entre procesos en un sistema embebido:
```c
shmget, shmat, shmdt, shmctl
```

---

Este documento proporciona una guía completa sobre **procesos e hilos en Linux embebido**, cubriendo desde su creación hasta su optimización y depuración.

