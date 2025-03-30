# Inspección de cuellos de botella con `htop` y `iotop`

## Objetivo:
Monitorear el uso de CPU y E/S en tiempo real.

## Pasos:
1. Instala las herramientas:
   ```bash
   sudo apt install htop iotop
   ```
2. Lanza `htop` y observa los procesos con mayor uso de CPU y RAM:
   ```bash
   htop
   ```
3. En otra terminal, genera carga:
   ```bash
   dd if=/dev/zero of=tempfile bs=1M count=500
   ```
4. Lanza `iotop` para ver el uso de disco:
   ```bash
   sudo iotop
   ```