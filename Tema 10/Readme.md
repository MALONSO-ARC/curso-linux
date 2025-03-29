# Tema 10: Procesos e Hilos en Linux Embebido

## 1. Introducción a los procesos en Linux

### ¿Qué es un proceso en Linux?

En Linux, un **proceso** es una instancia de un programa en ejecución. Cada vez que se ejecuta un binario, el kernel crea un nuevo proceso. Este contiene su propio espacio de memoria, registros de CPU, pila, y otros recursos necesarios para su ejecución. Los procesos son gestionados por el **scheduler** del kernel, que decide cuándo y durante cuánto tiempo se ejecutan.

### Particularidades en sistemas embebidos

En sistemas embebidos, los procesos funcionan igual que en un sistema Linux estándar, pero con **restricciones adicionales**:

- **Recursos limitados**: CPU de bajo consumo, poca RAM y almacenamiento limitado.
- **Requisitos de tiempo real**: Muchos sistemas embebidos tienen plazos estrictos que deben cumplirse.
- **Uso intensivo de procesos ligeros o hilos** para optimizar el rendimiento.
- **Configuración personalizada del sistema operativo**: se suele usar una versión reducida del sistema Linux (BusyBox, Buildroot, Yocto).

### Ciclo de vida de un proceso

1. **Creación**: Un proceso puede crear otro mediante `fork()` (duplicación) o `vfork()` (más eficiente en entornos embebidos).
2. **Ejecución**: El proceso hijo puede reemplazar su imagen mediante `exec()` para ejecutar un nuevo programa.
3. **Estado**: Un proceso puede estar en varios estados: ejecutando, esperando, detenido, etc.
4. **Terminación**: Un proceso finaliza mediante `exit()`, voluntariamente o por una señal.

> En sistemas embebidos es común evitar procesos innecesarios para conservar memoria y mejorar la determinación temporal.

---

### Procesos vs Demonios

Un **demonio** (daemon) es un proceso que se ejecuta en segundo plano, sin control directo del usuario (por ejemplo, `sshd`, `inetd`). En Linux embebido, los demonios son comunes para:

- Gestionar dispositivos (`udevd`)
- Monitorizar sensores
- Ejecutar tareas periódicas

En muchos casos, los desarrolladores embebidos crean sus propios demonios adaptados al hardware específico.

---

### Herramientas para observar procesos

En Linux embebido, debido a la simplicidad del entorno, muchas veces se utilizan versiones reducidas de herramientas. Algunas opciones:

| Herramienta  | Descripción                             |
|--------------|------------------------------------------|
| `ps`         | Lista los procesos en ejecución          |
| `top` o `htop` | Monitor interactivo del sistema         |
| `pidof`      | Muestra el PID de un proceso             |
| `/proc`      | Sistema de archivos virtual para examinar información detallada de cada proceso |

> En entornos embebidos minimalistas (como BusyBox), `ps` y `top` pueden no estar disponibles o tener funcionalidades limitadas.

---

### Buenas prácticas

- **Evitar fork innecesario**: el uso excesivo de `fork()` puede agotar los recursos.
- **Usar procesos livianos o hilos**: cuando sea posible, usar `pthread` o sistemas event-driven.
- **Supervisión y reinicio**: usar `init` o `systemd` (si está disponible) para reiniciar procesos críticos.
- **Evitar procesos zombis**: siempre recolectar el estado de los procesos hijos con `wait()`.



## 2. Creación y gestión de procesos con `fork()`

### ¿Qué es `fork()`?

La función `fork()` es una llamada al sistema que permite crear un nuevo proceso en Linux. Cuando se invoca, crea un **proceso hijo** duplicando casi por completo el proceso padre. Ambos procesos continuarán ejecutándose a partir del mismo punto después del `fork()`, pero con un PID diferente.

```c
#include <unistd.h>
pid_t fork(void);
```

- Retorna **0** al proceso hijo
- Retorna el **PID del hijo** al proceso padre
- Retorna **-1** si hay un error

> `fork()` es parte de la **libc**, la biblioteca estándar de C, que actúa como un envoltorio (wrapper) para realizar la llamada al sistema correspondiente en el kernel de Linux.

---

### Ejemplo básico de `fork()`

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

---

### Funciones relacionadas con `fork()`

#### `wait()` y `waitpid()`
Permiten que el proceso padre espere a que su hijo termine y evitar procesos zombis.

```c
#include <sys/wait.h>
int status;
wait(&status);       // Espera cualquier hijo
waitpid(pid, &status, 0); // Espera un hijo específico
```

- `wait()` bloquea hasta que cualquier hijo termine.
- `waitpid()` permite más control (por ejemplo, esperar de forma no bloqueante con `WNOHANG`).

#### `exec()` y variantes
Permiten reemplazar la imagen del proceso actual por otro programa:

```c
#include <unistd.h>
execlp("ls", "ls", "-l", NULL);
```

Variantes de `exec()`:

| Función      | Descripción                                   |
|--------------|-----------------------------------------------|
| `execl()`    | Lista de argumentos                          |
| `execlp()`   | Igual que `execl()`, pero busca en `PATH`    |
| `execle()`   | Como `execl()`, pero permite pasar entorno   |
| `execv()`    | Vector de argumentos                         |
| `execvp()`   | Vector + búsqueda en `PATH`                  |
| `execve()`   | Llamada al sistema más directa               |

> Todas son wrappers de la syscall `execve()` del kernel de Linux.

#### `getpid()` y `getppid()`

- `getpid()` devuelve el PID del proceso actual.
- `getppid()` devuelve el PID del padre del proceso actual.

---

### Consideraciones en sistemas embebidos

- `fork()` puede ser costoso en sistemas con poca memoria; considerar `vfork()` o usar hilos si es posible.
- Evitar dejar procesos zombis; siempre usar `wait()` o mecanismos similares.
- El uso de `exec()` es común en inicialización de demonios o scripts de sistema.

---

## 3. Introducción a hilos (`pthread`)

En sistemas Linux embebidos, el uso de hilos (o *threads*) permite ejecutar tareas concurrentes dentro de un mismo proceso, lo cual es especialmente valioso cuando se desea aprovechar múltiples núcleos del procesador o mantener tareas reactivas sin bloquear la ejecución del resto del programa.

### ¿Qué es un hilo?
Un hilo es una unidad de ejecución dentro de un proceso. A diferencia de los procesos, los hilos comparten el mismo espacio de memoria y recursos del proceso que los contiene. Esto hace que los hilos sean más ligeros y rápidos de crear y destruir que los procesos, aunque también implica que deben coordinarse cuidadosamente para evitar condiciones de carrera (*race conditions*) y otros problemas de concurrencia.

### Ventajas del uso de hilos
- **Paralelismo**: Permiten ejecutar código en paralelo en sistemas con más de un núcleo.
- **Reactividad**: Facilitan la creación de aplicaciones que reaccionan rápidamente a eventos (por ejemplo, interrupciones o E/S).
- **Eficiencia**: Compartir memoria reduce la sobrecarga de comunicación entre hilos frente a procesos separados.

### Desventajas y consideraciones
- **Sincronización**: Es necesario usar mecanismos como mutexes o semáforos para proteger el acceso a recursos compartidos.
- **Depuración**: Los errores de concurrencia pueden ser difíciles de detectar y reproducir.
- **Sobrecarga cognitiva**: Diseñar sistemas multihilo puede ser complejo.

### Librería `pthread`
En C, la librería POSIX `pthread` proporciona una API para trabajar con hilos. Es el estándar en sistemas Unix-like y también en Linux embebido.


---

## 4. Comunicación entre procesos: Pipes, FIFO, Shared Memory

La **comunicación entre procesos (IPC)** en Linux embebido es fundamental cuando varios procesos deben intercambiar información de manera eficiente. Existen diversos mecanismos disponibles, cada uno con sus ventajas, limitaciones y casos de uso particulares.

---

### 4.1 Pipes (Tubos anónimos)
Los **pipes anónimos** permiten la comunicación unidireccional entre procesos emparentados, como padre e hijo. Se crean con la función `pipe()` y usan descriptores de archivo.

#### Ejemplo:
```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int fd[2];
    char buffer[100];
    pipe(fd);

    if (fork() == 0) {
        // Proceso hijo: lee
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("[Hijo] Mensaje recibido: %s\n", buffer);
    } else {
        // Proceso padre: escribe
        close(fd[0]);
        char *msg = "Hola desde el padre";
        write(fd[1], msg, strlen(msg)+1);
    }
    return 0;
}
```

**Ventajas:**
- Simples de usar.
- Adecuados para procesos relacionados.

**Limitaciones:**
- No sirven para procesos no emparentados.
- Comunicación unidireccional.

---

### 4.2 FIFO (Named Pipes)
Los **FIFO** permiten la comunicación entre procesos no relacionados mediante un archivo especial en el sistema de archivos.

#### Creación y uso con comandos:
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

#### Ejemplo en C:
Escritura:
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/tmp/mi_fifo", O_WRONLY);
    write(fd, "Mensaje desde escritor", 24);
    close(fd);
    return 0;
}
```

Lectura:
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[100];
    int fd = open("/tmp/mi_fifo", O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("Mensaje recibido: %s\n", buffer);
    close(fd);
    return 0;
}
```

**Ventajas:**
- Comunicación entre procesos independientes.

**Limitaciones:**
- Persistencia en el sistema de archivos.
- Requiere coordinación externa para sincronizar lectura y escritura.

---

### 4.3 Memoria compartida (Shared Memory)
La **memoria compartida** es el mecanismo IPC más rápido. Permite que dos o más procesos accedan al mismo espacio de memoria. Es ideal para transferencias de datos grandes, pero requiere sincronización con semáforos o mutexes.

#### Ejemplo:
```c
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    char *shm = (char *) shmat(shmid, NULL, 0);
    sprintf(shm, "Mensaje en memoria compartida");
    printf("[Padre] Escribió: %s\n", shm);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
```

**Ventajas:**
- Muy eficiente en rendimiento.
- Ideal para datos grandes o compartidos constantemente.

**Limitaciones:**
- Requiere sincronización manual.
- Manejo más complejo que pipes o FIFO.

---

### 4.4 Sockets
Los **sockets** son un mecanismo muy flexible para IPC y comunicación entre máquinas. Soportan comunicación local (`AF_UNIX`) o sobre red (`AF_INET`).

#### Ejemplo (AF_UNIX):
Servidor:
```c
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#define SOCKET_PATH "/tmp/misocket"

int main() {
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr = {.sun_family = AF_UNIX};
    strcpy(addr.sun_path, SOCKET_PATH);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    int client_fd = accept(server_fd, NULL, NULL);
    char buffer[100];
    read(client_fd, buffer, sizeof(buffer));
    printf("[Servidor] Mensaje: %s\n", buffer);
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
```

Cliente:
```c
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#define SOCKET_PATH "/tmp/misocket"

int main() {
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr = {.sun_family = AF_UNIX};
    strcpy(addr.sun_path, SOCKET_PATH);
    connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));

    write(client_fd, "Hola desde cliente", 20);
    close(client_fd);
    return 0;
}
```

**Ventajas:**
- Versátiles: comunicación local y remota.
- Soportan múltiples conexiones.
- Bidireccionales y orientados a flujo o datagramas.

**Limitaciones:**
- Requiere más configuración.
- Leve sobrecarga comparado con mecanismos más simples.

---

### 4.5 Colas de mensajes (Message Queues)
Las **colas de mensajes** permiten enviar y recibir estructuras de datos en forma de mensajes entre procesos. Se basan en colas del sistema gestionadas por el kernel.

#### Ejemplo:
```c
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf message;
    message.mtype = 1;
    strcpy(message.mtext, "Hola desde cola de mensajes");

    msgsnd(msgid, &message, sizeof(message.mtext), 0);
    printf("Mensaje enviado\n");
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
```

**Ventajas:**
- Control estructurado de mensajes.
- Sincronización implícita.

**Limitaciones:**
- Máximo tamaño limitado por el sistema.
- Más lentas que memoria compartida.

---

### Comparativa rápida
| Mecanismo       | Dirección       | Procesos relacionados | Persistencia | Velocidad | Red     | Complejidad |
|-----------------|------------------|------------------------|--------------|-----------|---------|-------------|
| Pipe            | Unidireccional   | Sí                    | No           | Media     | No      | Baja        |
| FIFO            | Unidireccional   | No                    | Sí           | Media     | No      | Baja        |
| Shared Memory   | Bidireccional    | Sí/No                 | No           | Alta      | No      | Alta        |
| Socket          | Bidireccional    | Sí/No                 | Opcional     | Media     | Sí/No   | Media       |
| Message Queue   | Bidireccional    | Sí/No                 | No           | Media     | No      | Media       |

---

### Análisis de uso habitual y tendencias
- **Pipes y FIFO**: se siguen utilizando por su simplicidad en entornos controlados o scripts, pero están en desuso en aplicaciones complejas.
- **Memoria compartida**: muy usada en sistemas embebidos de alto rendimiento, pero conlleva complejidad en sincronización.
- **Sockets**: ampliamente utilizados, especialmente en aplicaciones cliente-servidor, tanto locales como en red.
- **Message Queues**: populares en sistemas donde se requiere organización de mensajes estructurados. Son una buena alternativa a memoria compartida cuando se busca simplicidad con orden.

En general, **sockets y memoria compartida** son los mecanismos más utilizados en sistemas modernos por su flexibilidad y rendimiento, respectivamente.

---

## 5. Sincronización de hilos: Mutex y Semáforos

Cuando varios hilos acceden a recursos compartidos en un sistema embebido, es imprescindible aplicar mecanismos de **sincronización** para evitar errores como condiciones de carrera, corrupción de datos y bloqueos.

---

### 5.1 Mutex (Mutual Exclusion)
Un **mutex** es un objeto que permite que un solo hilo acceda a una sección crítica del código al mismo tiempo. Es el mecanismo más común para proteger recursos compartidos.

#### Inicialización y uso básico:
```c
#include <pthread.h>

pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);

pthread_mutex_lock(&lock);
// Sección crítica
pthread_mutex_unlock(&lock);

pthread_mutex_destroy(&lock);
```

#### Características:
- Evita que más de un hilo entre en la sección crítica.
- Requiere que el mismo hilo que hace `lock` haga `unlock`.
- Puede causar **deadlocks** si no se gestiona correctamente.

#### Ejemplo:
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;
int contador = 0;

void *incrementar(void *arg) {
    for (int i = 0; i < 10000; i++) {
        pthread_mutex_lock(&mutex);
        contador++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t h1, h2;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&h1, NULL, incrementar, NULL);
    pthread_create(&h2, NULL, incrementar, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);

    pthread_mutex_destroy(&mutex);
    printf("Contador final: %d\n", contador);
    return 0;
}
```

---

### 5.2 Semáforos
Los **semáforos** permiten gestionar el acceso concurrente mediante contadores. Son útiles tanto para sincronización entre hilos como entre procesos.

#### Tipos:
- **Semáforos binarios**: valor 0 o 1 (equivalente a un mutex).
- **Semáforos contadores**: valor entero mayor o igual a 0.

#### Inicialización y uso básico:
```c
#include <semaphore.h>

sem_t sem;
sem_init(&sem, 0, 1);

sem_wait(&sem);
// Sección crítica
sem_post(&sem);

sem_destroy(&sem);
```

- `sem_init(sem, pshared, valor_inicial)`:
  - `pshared = 0` para hilos del mismo proceso.
  - `pshared = 1` para procesos distintos (requiere memoria compartida).

#### Ejemplo:
```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t sem;
int recurso = 0;

void *acceso(void *arg) {
    for (int i = 0; i < 10000; i++) {
        sem_wait(&sem);
        recurso++;
        sem_post(&sem);
    }
    return NULL;
}

int main() {
    pthread_t h1, h2;
    sem_init(&sem, 0, 1);

    pthread_create(&h1, NULL, acceso, NULL);
    pthread_create(&h2, NULL, acceso, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);

    sem_destroy(&sem);
    printf("Valor final: %d\n", recurso);
    return 0;
}
```

---

### Comparación Mutex vs Semáforo
| Característica | Mutex                   | Semáforo               |
|-----------------|--------------------------|--------------------------|
| Tipo            | Exclusión mutua (binario)| Binario o contador       |
| Ámbito          | Hilos del mismo proceso  | Hilos o procesos         |
| Propietario     | Sí                      | No                       |
| Desbloqueo      | Solo el hilo que bloquea | Cualquier hilo o proceso |
| Riesgo de uso   | Menor                    | Mayor (más flexibilidad) |

---

Estos mecanismos son fundamentales para garantizar la **consistencia** y **estabilidad** del sistema cuando varios hilos deben acceder a recursos compartidos en Linux embebido.


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

