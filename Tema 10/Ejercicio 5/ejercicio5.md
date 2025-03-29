## Ejercicio: Sincronización de hilos con Mutex y Semáforo

**Objetivo:** Comprender el uso de mutex y semáforos para proteger el acceso a una variable compartida entre varios hilos.

### Enunciado

Escribe un programa en C que:
- Cree 2 hilos.
- Cada hilo incremente una variable compartida 10,000 veces.
- Use un **mutex** para evitar condiciones de carrera.
- Luego, repite el mismo programa pero usando un **semáforo binario** en lugar del mutex.

### Parte 1: Usando Mutex

```c
#include <pthread.h>
#include <stdio.h>

#define ITERACIONES 10000

int contador = 0;
pthread_mutex_t lock;

void *incrementar(void *arg) {
    for (int i = 0; i < ITERACIONES; i++) {
        pthread_mutex_lock(&lock);
        contador++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, incrementar, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    printf("Contador final (mutex): %d\n", contador);
    return 0;
}
```

### Parte 2: Usando Semáforo

```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define ITERACIONES 10000

int contador = 0;
sem_t sem;

void *incrementar(void *arg) {
    for (int i = 0; i < ITERACIONES; i++) {
        sem_wait(&sem);
        contador++;
        sem_post(&sem);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    sem_init(&sem, 0, 1);

    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, incrementar, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);
    printf("Contador final (semáforo): %d\n", contador);
    return 0;
}
```

### Resultados esperados
- En ambos casos, el valor final del contador debe ser 20,000.
- Sin sincronización, el valor puede variar y ser incorrecto debido a condiciones de carrera.

### Preguntas de reflexión
1. ¿Qué ocurriría si se elimina el `mutex` o el `semáforo` del código?
2. ¿Cuáles son las diferencias prácticas al usar mutex frente a semáforos en este ejemplo?
3. ¿Cuándo elegirías usar semáforos sobre mutexes y viceversa?

