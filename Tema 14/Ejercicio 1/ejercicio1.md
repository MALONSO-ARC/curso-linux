#  Identificar procesos que consumen CPU

**Objetivo:** Utilizar `top` y `htop` para detectar procesos con alto consumo de CPU.

## Pasos:
1. Abre una terminal y lanza una carga de CPU artificial:
```bash
yes > /dev/null &
```
2. Ejecuta:
```bash
top
```
3. Observa el proceso `yes` y su consumo de CPU.
4. Ejecuta:
```bash
htop
```
5. Usa las teclas de ordenamiento (`P`, `M`) y mata el proceso desde `htop` (`F9`).

**Preguntas:**
- ¿Cuál fue el consumo máximo observado?
- ¿Qué diferencias notas entre `top` y `htop`?



