### Ejercicio práctico: Lanzamiento controlado de comandos

**Objetivo**: Entender el uso combinado de `fork()`, `exec()` y `wait()` para controlar procesos.

#### Enunciado

Escribe un programa en C que:

1. Cree un proceso hijo usando `fork()`.
2. El proceso hijo debe ejecutar el comando `uname -a` usando `execlp()`.
3. El padre debe esperar a que el hijo termine e imprimir su estado de salida.


#### Preguntas para reflexionar

- ¿Qué pasa si `execlp()` falla? ¿Cómo lo detectas?
- ¿Qué ocurre si no llamas a `wait()` en el padre?
- ¿Cómo podrías modificar este programa para lanzar varios comandos secuencialmente?

---