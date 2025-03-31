# Auditoría básica del sistema

**Objetivo:** Evaluar el estado de seguridad con herramientas de auditoría.

## Pasos:
1. **Instalar herramientas:**
   ```bash
   sudo apt install lynis checksec elfutils clamav pax-utils
   ```

2. **Ejecutar auditoría:**
   ```bash
   sudo lynis audit system
   ```

3. **Analizar un binario compilado:**
   ```bash
   gcc -o hola hola.c
   checksec --file=hola
   ```

4. **Buscar funciones peligrosas:**
   ```bash
   scanelf -qR /usr/bin | grep 'libc.so'
   ```

5. **Escanear con `clamav`:**
   ```bash
   clamscan -r /usr/bin
   ```