# Pthread Cheatsheet (Hilos POSIX en C)

## Creación y gestión de hilos

```c
#include <pthread.h>

pthread_t thread;
pthread_create(&thread, NULL, funcion, arg);
```
- `pthread_t`: tipo de dato para un identificador de hilo.
- `pthread_create`: crea un hilo.
  - 1º: puntero al hilo
  - 2º: atributos (NULL = por defecto)
  - 3º: función que ejecuta el hilo (`void *func(void *)`)
  - 4º: argumento que se pasa a la función

## Esperar a que termine un hilo

```c
pthread_join(thread, NULL);
```
- Espera a que el hilo termine antes de continuar.

## Terminar un hilo manualmente

```c
pthread_exit(NULL);
```
- Finaliza la ejecución del hilo actual.

## Detach de un hilo (ejecución independiente)

```c
pthread_detach(thread);
```
- El hilo libera sus recursos automáticamente al finalizar.

## Paso de argumentos

```c
void *func(void *arg) {
    int valor = *(int *)arg;
    ...
}

int dato = 42;
pthread_create(&thread, NULL, func, &dato);
```
- Usa punteros para pasar datos a la función del hilo.

## Sincronización con mutex

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&lock);
// Sección crítica
pthread_mutex_unlock(&lock);
```
- Protege secciones críticas para evitar condiciones de carrera.

## Inicialización manual de mutex

```c
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
```

## Destrucción de mutex

```c
pthread_mutex_destroy(&lock);
```

## Crear múltiples hilos en un bucle

```c
#define N 5
pthread_t hilos[N];
for (int i = 0; i < N; i++) {
    pthread_create(&hilos[i], NULL, funcion, NULL);
}
```

---

## Semáforos con POSIX (`semaphore.h`)

### Inclusión y tipos
```c
#include <semaphore.h>
sem_t sem;
```

### Inicialización
```c
sem_init(&sem, 0, 1); // 0 = uso entre hilos (no entre procesos)
```

### Esperar y liberar
```c
sem_wait(&sem);   // Decrementa y bloquea si el valor es 0
// Sección crítica
sem_post(&sem);   // Incrementa el semáforo
```

### Destrucción
```c
sem_destroy(&sem);
```

### Ejemplo completo
```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t sem;

void *func(void *arg) {
    sem_wait(&sem);
    printf("Hilo en sección crítica\n");
    sem_post(&sem);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    sem_init(&sem, 0, 1);

    pthread_create(&t1, NULL, func, NULL);
    pthread_create(&t2, NULL, func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);
    return 0;
}
```

**Ventajas:** útiles para controlar acceso entre múltiples hilos cuando solo cierta cantidad debe entrar en una sección crítica.

---

## Buenas prácticas
- Usa `pthread_join` para evitar procesos zombie.
- Protege recursos compartidos con mutex o semáforos.
- Evita variables locales que puedan dejar de existir al terminar `main()`.
- Cuidado con el uso de variables globales compartidas.
