## Ejercicio 2: Configuración de prioridades con `chrt`

**Objetivo:** Ejecutar tareas con política `SCHED_FIFO` y verificar su comportamiento.

### Pasos:
1. Crea un script que imprima su PID y duerma:
```bash
echo -e "#!/bin/bash\necho PID: $$\nsleep 60" > tarea.sh
chmod +x tarea.sh
```
2. Ejecuta el script con prioridad FIFO:
```bash
sudo chrt -f 90 ./tarea.sh
```
3. En otra terminal, verifica la política de planificación:
```bash
ps -o pid,cmd,policy | grep tarea
```

**Preguntas:**
- ¿Qué política aparece?
- ¿Qué pasa si ejecutas una segunda tarea con mayor prioridad?