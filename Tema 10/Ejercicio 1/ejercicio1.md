# Creación y análisis de un proceso en Linux embebido

**Objetivo**: Comprender la creación, ejecución y finalización de procesos.

## Instrucciones

1. En tu entorno, crea un pequeño programa en C que:
   - Use `fork()` para crear un proceso hijo.
   - El proceso hijo ejecute `ls` usando `execlp()`.
   - El padre espere a que termine el hijo usando `wait()`.

2. Compila el programa con `gcc`:

```bash
gcc -o test_process test_process.c
```

3. Ejecuta el binario y observa la salida.
4. Abre otra terminal y usa `ps` para observar los procesos.
5. Verifica en `/proc/<pid>` la información del proceso hijo mientras está vivo.

## Reflexión

- ¿Qué pasa si ejecutas múltiples veces seguidas este programa?
- ¿Qué ocurre si omites `wait()` en el padre?