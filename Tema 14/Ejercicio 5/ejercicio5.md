# Ejercicio 5: Captura de eventos avanzados con `LTTng`

**Objetivo:** Usar `LTTng` para capturar eventos del kernel mientras se ejecuta una aplicación.

## Pasos:
1. Instala las herramientas:
```bash
sudo apt install lttng-tools lttng-modules-dkms babeltrace
```
2. Crea y lanza una sesión:
```bash
lttng create prueba
lttng enable-event -a -k
lttng start
```
3. Ejecuta una aplicación de prueba:
```bash
ls -lR /
```
4. Detén la captura y visualiza:
```bash
lttng stop
lttng view
```
5. Visualiza con:
```bash
babeltrace ~/lttng-traces/prueba-*
```

**Preguntas:**
- ¿Qué eventos del kernel se registraron?
- ¿Qué procesos estaban activos durante la captura?