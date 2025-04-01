# Compilación de un kernel con PREEMPT-RT (simulado)

**Objetivo:** Entender los pasos para compilar un kernel RT (no se requiere ejecutar la compilación completa).

## Pasos:
1. Crea una carpeta temporal:
```bash
mkdir ~/kernel-rt && cd ~/kernel-rt
```
2. Descarga el código fuente del kernel:
```bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.12.tar.xz
```
3. Extrae y explora la configuración:
```bash
tar -xf linux-6.12.tar.xz
cd linux-6.12
make menuconfig
```
4. Busca las siguientes opciones:
   - Fully Preemptible Kernel (Real-Time)
   - High Resolution Timers

**Preguntas:**
- ¿Dónde se activan estas opciones?
- ¿Qué implicaciones tiene compilar con `CONFIG_PREEMPT_RT` habilitado?