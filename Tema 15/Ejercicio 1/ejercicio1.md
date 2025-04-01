# Ejercicios Tema 15: Linux en Tiempo Real

Estos ejercicios están diseñados para ejecutarse en una máquina virtual Ubuntu 22.04 (o similar) configurada con un kernel PREEMPT-RT o con Xenomai.

---

## Ejercicio 1: Medición de latencias con `cyclictest`

**Objetivo:** Verificar la latencia y el jitter en un sistema Linux con soporte tiempo real.

### Pasos:
1. Instala la herramienta:
```bash
sudo apt install rt-tests
```
2. Ejecuta el test:
```bash
sudo cyclictest -t1 -p80 -n -i 1000 -l 100000
```
3. Observa los valores de latencia mínima, máxima y promedio.

**Preguntas:**
- ¿Qué jitter obtuviste?
- ¿El sistema se comporta de forma determinista?

