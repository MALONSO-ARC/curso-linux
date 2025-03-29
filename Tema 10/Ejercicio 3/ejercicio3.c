#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *tareaA(void *arg) {
    for (int i = 0; i < 5; i++) {
        printf("[1] Ejecutando tarea A\n");
        sleep(1);
    }
    return NULL;
}

void *tareaB(void *arg) {
    for (int i = 0; i < 5; i++) {
        printf("[2] Ejecutando tarea B\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2;

    pthread_create(&hilo1, NULL, tareaA, NULL);
    pthread_create(&hilo2, NULL, tareaB, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}