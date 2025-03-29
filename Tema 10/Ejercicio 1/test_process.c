#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo
        printf("Hijo: PID = %d\n", getpid());
        execlp("ls", "ls", "-l", "/", NULL);
        perror("execlp falló");
    } else if (pid > 0) {
        // Proceso padre
        printf("Padre: esperando al hijo %d\n", pid);
        wait(NULL);
        printf("Padre: el hijo terminó\n");
    } else {
        perror("fork falló");
    }

    return 0;
}