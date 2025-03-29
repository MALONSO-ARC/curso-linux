#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    int status;

    if (pid < 0) {
        perror("Error al hacer fork");
        exit(1);
    } else if (pid == 0) {
        execlp("uname", "uname", "-a", NULL);
        perror("execlp falló");
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("El hijo terminó con estado %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}