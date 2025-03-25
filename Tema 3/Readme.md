# Tema 3: Repaso de Linux

## 1. Instalación de una distribución Linux para desarrollo
Para trabajar con Linux embebido, es recomendable usar una distribución de escritorio para desarrollo. Algunas opciones populares:

- **Ubuntu/Debian**: Fáciles de usar y con amplio soporte.
- **Arch Linux**: Ligero y personalizable.
- **Fedora**: Moderno y actualizado.

### Instalación en una máquina virtual:
1. Descargar la imagen ISO de la distribución elegida.
2. Usar VirtualBox o QEMU para crear una máquina virtual.
3. Configurar almacenamiento y red.
4. Instalar el sistema y actualizar paquetes con `apt update && apt upgrade`.

## 2. Configuración de un entorno de desarrollo (VMs, contenedores)
Opciones para el entorno de desarrollo:

- **Máquinas Virtuales (VMs)**: Aíslan el entorno sin afectar el sistema host.
- **Contenedores (Docker, Podman)**: Livianos y portables. Son una muy buena opción para ser usados tanto en el desarrollo (unifica el mismo entorno entre desarrolladores) como apra ser usado en pipelines de CI/CD

Ejemplo de creación de un contenedor con Docker:
```bash
docker pull ubuntu:22.04

docker run -it --name ubuntu_container ubuntu:22.04 bash
```

## 3. Comandos básicos de administración
Algunos comandos esenciales en Linux:

- **Navegación y gestión de archivos:**
  ```bash
  ls      # Listar archivos
  cd      # Cambiar directorio
  rm      # Eliminar archivos
  cp      # Copiar archivos
  mv      # Mover archivos
  chmod   # Cambiar permisos
  ```

## 4. Gestión de procesos
  ```bash
  ps      # Listar procesos activos
  top     # Monitorizar procesos en tiempo real
  kill    # Finalizar procesos
  ```

## 5. Control de permisos y usuarios
- **`chmod`**: Cambiar permisos de archivos.
- **`chown`**: Cambiar propietario de archivos.
- **`sudo`**: Ejecutar comandos como superusuario.

Ejemplo:
```bash
chmod +x script.sh    # Hacer un archivo ejecutable
chown usuario:grupo archivo.txt  # Cambiar propietario
```

## 6. Configuración y diagnóstico de red
Comandos útiles para gestionar redes:

```bash
ifconfig  # Mostrar configuración de red
ip addr   # Ver direcciones IP
ping      # Verificar conectividad
netstat   # Mostrar conexiones de red activas
```

## 7. Uso de editores de texto en entornos embebidos
Los editores más utilizados en Linux embebido son:

- **Vim**: Potente, pero con curva de aprendizaje.
- **Nano**: Más fácil de usar.
- **Emacs**: Completo, con extensiones avanzadas.

Ejemplo de edición con Vim:
```bash
vim archivo.txt
# Presionar "i" para editar, "Esc" para salir del modo edición y ":wq" para guardar y salir.
```

## 8. Introducción a scripting en Bash y automatización de tareas

El shell Bash es una herramienta fundamental en sistemas Linux embebidos, no solo como interfaz de comandos, sino también como lenguaje de scripting. Con Bash es posible automatizar tareas repetitivas, configurar dispositivos al inicio, realizar diagnósticos del sistema y hasta controlar hardware.

Esta sección explora los fundamentos del scripting en Bash con un enfoque práctico para sistemas embebidos.



### Fundamentos del scripting en Bash

#### 🔎 Estructura básica de un script
Un script Bash es un archivo de texto que contiene comandos que pueden ejecutarse como un programa:
```bash
#!/bin/bash

echo "Hola desde Bash"
```
- La primera línea `#!/bin/bash` es el *shebang* e indica al sistema que use Bash para interpretar el script.

#### ✅ Permisos de ejecución
Un script debe tener permisos de ejecución para poder correr:
```bash
chmod +x script.sh
./script.sh
```

---

### Variables, condiciones y bucles

#### 🔢 Variables
```bash
NOMBRE="BeagleBone"
echo "Dispositivo: $NOMBRE"
```

#### ⚖️ Condicionales
```bash
if [ "$1" = "on" ]; then
    echo "Encendiendo..."
else
    echo "Parando..."
fi
```

#### ⟳ Bucles
```bash
for i in 1 2 3; do
    echo "Iteración $i"
done

while true; do
    echo "Bucle infinito"
    sleep 1
done
```

---

### Scripts comunes en sistemas embebidos

#### ⚙️ Inicialización de periféricos
```bash
#!/bin/bash
echo 17 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio17/direction
echo 1 > /sys/class/gpio/gpio17/value
```

#### 📊 Monitoreo de recursos
```bash
#!/bin/bash
free -h > /tmp/memoria.txt
df -h >> /tmp/memoria.txt
echo "Uso de recursos guardado"
```

#### 🚀 Automatización de despliegue
```bash
#!/bin/bash
scp firmware.bin root@192.168.1.10:/opt/
ssh root@192.168.1.10 "systemctl restart firmware"
```

---

### Automatización con cron

#### 🕒 Programación de tareas periódicas
`cron` permite ejecutar scripts de manera automática en intervalos definidos.

Editar tareas:
```bash
crontab -e
```

Ejemplo: ejecutar un script cada 5 minutos
```
*/5 * * * * /home/usuario/monitorear.sh
```

Ver tareas:
```bash
crontab -l
```

---

### Automatización al arranque del sistema

#### 🚪 Uso de systemd (moderno)
Crear archivo `/etc/systemd/system/gpio-init.service`:
```ini
[Unit]
Description=Inicializar GPIO
After=multi-user.target

[Service]
ExecStart=/usr/local/bin/gpio-init.sh

[Install]
WantedBy=multi-user.target
```

Activar el servicio:
```bash
sudo systemctl daemon-reexec
sudo systemctl enable gpio-init
sudo systemctl start gpio-init
```

#### 🔹 Alternativas:
- `/etc/rc.local` (deprecated pero funcional).
- Scripts en `/etc/init.d/` (SysVinit).

---

### Buenas prácticas en scripting embebido

- Validar entradas del usuario.
- Usar `set -e` para salir en errores.
- Comprobar dispositivos antes de actuar:
  ```bash
  if [ -e /dev/mtd0 ]; then ...
  ```
- Redirigir logs a archivos: `>> /var/log/script.log`.
- Usar funciones para modularizar tareas.

---

### Ejemplo completo: script de backup diario
```bash
#!/bin/bash
# backup.sh - Realiza backup de /etc
FECHA=$(date +%Y%m%d)
DEST=/mnt/backup/etc-$FECHA.tar.gz

tar czf $DEST /etc && echo "Backup creado: $DEST"
```

Ejecutar desde `cron` cada noche:
```
0 3 * * * /usr/local/bin/backup.sh
```

---

### 🔗 Recursos adicionales

- [Guía oficial de Bash](https://www.gnu.org/software/bash/manual/bash.html)
- [Advanced Bash Scripting Guide (TLDP)](https://tldp.org/LDP/abs/html/)
- Herramientas útiles: `shellcheck`, `bash-completion`, `expect`, `dialog`

### 📝 Conclusión
El scripting en Bash es una herramienta poderosa y versátil para automatizar tareas en sistemas Linux embebido. Desde configuraciones al arranque, hasta despliegue remoto o diagnósticos automáticos, dominar Bash permite mejorar la eficiencia, confiabilidad y mantenibilidad de un sistema embebido.



## Uso de Git y otros sistemas de control de versiones
Git es esencial para el desarrollo en Linux embebido.

### Comandos básicos de Git:
```bash
git init                 # Inicializar un repositorio
git clone URL            # Clonar un repositorio
git add archivo          # Añadir archivo al índice
git commit -m "Mensaje"  # Guardar cambios
git push origin main     # Enviar cambios al repositorio remoto
```

## Manejo de logs y depuración de errores en Linux
El análisis de logs es clave para encontrar errores:

- **`dmesg`**: Ver registros del sistema.
- **`journalctl`**: Examinar logs del sistema (en distros con systemd).
- **`syslog`**: Archivos de logs en `/var/log`.

Ejemplo:
```bash
dmesg | grep error  # Buscar errores en logs del kernel
journalctl -xe      # Ver mensajes de error recientes
```

Nota: Aquí tienes una "cheatsheet" con los comandos de Linux más usados.
[Linux Commands CheatSheet](../assets/linux_commands_cheatsheet.pdf)