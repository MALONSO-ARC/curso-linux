# Trazar llamadas del kernel con `ftrace`

**Objetivo:** Usar `ftrace` para capturar funciones del kernel que se ejecutan durante una operación simple.

## Pasos:
1. Habilita `ftrace`:
```bash
echo function > /sys/kernel/debug/tracing/current_tracer
```
2. Limpia el buffer:
```bash
echo > /sys/kernel/debug/tracing/trace
```
3. Lanza un comando:
```bash
ls /
```
4. Revisa las trazas:
```bash
cat /sys/kernel/debug/tracing/trace | head -n 20
```
5. Desactiva `ftrace`:
```bash
echo nop > /sys/kernel/debug/tracing/current_tracer
```

**Preguntas:**
- ¿Qué funciones aparecen en la traza?
- ¿Qué tipo de información se obtiene?