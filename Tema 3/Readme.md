# Tema 3: Repaso de Linux

## Instalación de una distribución Linux para desarrollo
Para trabajar con Linux embebido, es recomendable usar una distribución de escritorio para desarrollo. Algunas opciones populares:

- **Ubuntu/Debian**: Fáciles de usar y con amplio soporte.
- **Arch Linux**: Ligero y personalizable.
- **Fedora**: Moderno y actualizado.

### Instalación en una máquina virtual:
1. Descargar la imagen ISO de la distribución elegida.
2. Usar VirtualBox o QEMU para crear una máquina virtual.
3. Configurar almacenamiento y red.
4. Instalar el sistema y actualizar paquetes con `apt update && apt upgrade`.

## Configuración de un entorno de desarrollo (VMs, contenedores)
Opciones para el entorno de desarrollo:

- **Máquinas Virtuales (VMs)**: Aíslan el entorno sin afectar el sistema host.
- **Contenedores (Docker, Podman)**: Livianos y portables.

Ejemplo de creación de un contenedor con Docker:
```bash
docker run -it --rm ubuntu:latest /bin/bash
```

## Comandos básicos de administración
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

- **Gestión de procesos:**
  ```bash
  ps      # Listar procesos activos
  top     # Monitorizar procesos en tiempo real
  kill    # Finalizar procesos
  ```

## Control de permisos y usuarios
- **`chmod`**: Cambiar permisos de archivos.
- **`chown`**: Cambiar propietario de archivos.
- **`sudo`**: Ejecutar comandos como superusuario.

Ejemplo:
```bash
chmod +x script.sh    # Hacer un archivo ejecutable
chown usuario:grupo archivo.txt  # Cambiar propietario
```

## Configuración y diagnóstico de red
Comandos útiles para gestionar redes:

```bash
ifconfig  # Mostrar configuración de red
ip addr   # Ver direcciones IP
ping      # Verificar conectividad
netstat   # Mostrar conexiones de red activas
```

## Uso de editores de texto en entornos embebidos
Los editores más utilizados en Linux embebido son:

- **Vim**: Potente, pero con curva de aprendizaje.
- **Nano**: Más fácil de usar.
- **Emacs**: Completo, con extensiones avanzadas.

Ejemplo de edición con Vim:
```bash
vim archivo.txt
# Presionar "i" para editar, "Esc" para salir del modo edición y ":wq" para guardar y salir.
```

## Introducción a scripting en Bash y automatización de tareas
Bash permite automatizar tareas con scripts.

Ejemplo de script:
```bash
#!/bin/bash
echo "Hola, mundo"
ls -l
```

Hacer el script ejecutable:
```bash
chmod +x script.sh
./script.sh
```

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

---
**Fin del Tema 3**
