# Ejercicio 4: Medición de latencias con `latencytop`

**Objetivo:** Medir qué procesos generan mayor latencia en el sistema.

## Pasos:
1. Instala la herramienta:
```bash
sudo apt install latencytop
```
2. Ejecuta en modo root:
```bash
sudo latencytop
```
3. Observa los motivos de latencia más comunes.

**Actividad adicional:**
- Ejecuta en otra terminal:
```bash
find / > /dev/null
```
- Observa si `latencytop` muestra nuevos eventos relacionados con I/O.

**Preguntas:**
- ¿Qué syscall tiene más latencia?
- ¿Qué proceso la genera?