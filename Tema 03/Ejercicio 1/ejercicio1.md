# Ejercicios Prácticos - Tema 3: Repaso de Linux

Estos ejercicios están diseñados para realizarse dentro de una máquina virtual Ubuntu 22.04, como parte del entorno de desarrollo del curso de Linux embebido.

---

## 🧪 Ejercicio 1: Automatiza la generación de backups

### Objetivo
Aprender a crear un script en Bash para generar copias de seguridad automáticas de archivos importantes del sistema.

### Instrucciones
1. Crea un directorio `~/scripts` y dentro de él un archivo llamado `backup.sh`.
2. El script debe:
   - Crear una copia comprimida del directorio `/etc`.
   - Guardarla en el directorio `~/backups` con nombre `etc-YYYYMMDD.tar.gz`.
3. Asegúrate de que el script sea ejecutable y prrate de que el script sea ejecutable y pr\u00fuebalo manualmente.
4. Automatiza su ejecución diaria a las 03:00 AM usando `cron`.


