# Comunicación bidireccional con sockets Unix (AF_UNIX)

**Objetivo:** Implementar un sistema cliente-servidor usando sockets locales (AF_UNIX) donde:
- El cliente envía un mensaje al servidor.
- El servidor imprime el mensaje recibido y responde.
- El cliente imprime la respuesta del servidor.

**Requisitos:**
- Comunicación bidireccional en un solo socket.
- Uso de `read()` y `write()` desde ambos lados.

**1. Servidor (servidor_socket.c):**
```c
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/misocket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[128];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);  // Eliminar si ya existe
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    printf("Servidor esperando conexiones en %s...\n", SOCKET_PATH);

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    read(client_fd, buffer, sizeof(buffer));
    printf("[Servidor] Mensaje recibido: %s\n", buffer);

    const char *respuesta = "Recibido. Saludos desde el servidor.";
    write(client_fd, respuesta, strlen(respuesta) + 1);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
```

**2. Cliente (cliente_socket.c):**
```c
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/misocket"

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[128];

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    const char *mensaje = "Hola desde el cliente";
    write(client_fd, mensaje, strlen(mensaje) + 1);

    read(client_fd, buffer, sizeof(buffer));
    printf("[Cliente] Respuesta del servidor: %s\n", buffer);

    close(client_fd);
    return 0;
}
```

**Instrucciones para probar:**
1. Compila ambos programas:
```bash
gcc servidor_socket.c -o servidor
gcc cliente_socket.c -o cliente
```
2. Ejecuta el servidor en una terminal:
```bash
./servidor
```
3. Ejecuta el cliente en otra terminal:
```bash
./cliente
```

**Resultado esperado:**
- El servidor imprime el mensaje recibido del cliente.
- El cliente imprime la respuesta del servidor.

