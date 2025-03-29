### Ejercicio práctico
**Objetivo:** Crear un programa que lance dos hilos:
1. Un hilo que imprima cada segundo "[1] Ejecutando tarea A"
2. Otro hilo que imprima cada segundo "[2] Ejecutando tarea B"

Ambos hilos deben ejecutarse durante 5 segundos y luego terminar.

#### Requisitos:
- Utiliza `pthread_create` para iniciar ambos hilos.
- Usa `sleep(1)` para pausar un segundo entre impresiones.
- Sincroniza el final del programa principal con `pthread_join`.

#### Pista:
Puedes utilizar un bucle dentro de cada función que ejecuta el hilo para repetir las impresiones.

#### Extensión opcional:
Agrega un mutex para proteger el acceso a `printf`, aunque en este caso simple no es estrictamente necesario. Esto sirve como práctica para el uso de sincronización entre hilos.

---

Este tipo de ejercicios permiten familiarizarse con los conceptos básicos de multihilo en C bajo Linux, una habilidad clave en muchos sistemas embebidos que requieren concurrencia eficiente sin recurrir a procesos pesados.
