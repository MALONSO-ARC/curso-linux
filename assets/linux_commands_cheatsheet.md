# 📌 Linux Embebido - Cheat Sheet de Comandos Esenciales

## 🖥️ Comandos Básicos del Sistema
```bash
pwd                      # Mostrar el directorio actual
ls -l                    # Listar archivos con detalles
cd <dir>                 # Cambiar de directorio
cp <origen> <destino>    # Copiar archivos o directorios
mv <origen> <destino>    # Mover o renombrar archivos
rm -rf <archivo/directorio>  # Eliminar archivos o directorios (¡con cuidado!)
find <ruta> -name <patrón>   # Buscar archivos por nombre
```

## 📂 Gestión de Archivos y Directorios
```bash
df -h                    # Ver uso de disco
du -sh <directorio>      # Ver tamaño de un directorio
mount /dev/sdX /mnt      # Montar un dispositivo
umount /mnt              # Desmontar un dispositivo
mkfs.ext4 /dev/sdX       # Formatear como ext4
```

## 🏗️ Compilación y Cross-compilación
```bash
make                     # Compilar usando Makefile
make clean               # Limpiar compilación
gcc -o salida fuente.c   # Compilar un archivo C
arm-linux-gnueabi-gcc -o salida fuente.c  # Cross-compilar para ARM
file <binario>           # Ver arquitectura de un binario
```

## 🔗 Gestión de Procesos
```bash
ps aux                   # Ver procesos en ejecución
top                      # Monitorear procesos en tiempo real
htop                     # Versión mejorada de top
kill -9 <PID>            # Terminar un proceso por ID
killall <nombre>         # Terminar todos los procesos con ese nombre
nice -n <valor> <comando>  # Ejecutar un proceso con prioridad ajustada
```

## ⚙️ Gestión de Servicios y Daemons
```bash
systemctl status <servicio>    # Ver estado de un servicio
systemctl start <servicio>     # Iniciar un servicio
systemctl stop <servicio>      # Detener un servicio
systemctl restart <servicio>   # Reiniciar un servicio
```

## 🛠️ Depuración y Kernel
```bash
dmesg | tail -n 50          # Ver últimos mensajes del kernel
cat /proc/cpuinfo           # Información del CPU
cat /proc/meminfo           # Información de la memoria
lsmod                       # Listar módulos cargados en el kernel
modprobe <módulo>           # Cargar un módulo del kernel
rmmod <módulo>              # Remover un módulo del kernel
```

## 🔌 Gestión de Dispositivos y Hardware
```bash
lsblk                       # Listar dispositivos de almacenamiento
fdisk -l                    # Información de particiones
i2cdetect -y 1              # Detectar dispositivos I2C
gpio readall                # Ver estado de pines GPIO (en Raspberry Pi)
cat /sys/class/thermal/thermal_zone0/temp  # Leer temperatura del CPU
```

## 🌐 Redes y Conectividad
```bash
ip a                        # Ver interfaces de red
ifconfig                    # (Obsoleto) Información de interfaces de red
ping <host>                 # Enviar paquetes de prueba
netstat -tulnp              # Ver puertos abiertos y conexiones activas
scp <archivo> usuario@host:/ruta  # Copiar archivos por SSH
```

## 🔥 Gestión de U-Boot
```bash
printenv                    # Ver variables de entorno de U-Boot
setenv bootargs "console=ttyS0,115200 root=/dev/mmcblk0p2"  # Configurar variables de arranque
saveenv                     # Guardar cambios en U-Boot
bootm 0x81000000            # Iniciar kernel desde una dirección de memoria
```

## 📜 Sistema de Archivos y NFS
```bash
mount -o nolock -t nfs 192.168.1.100:/nfsroot /mnt  # Montar NFS
umount /mnt              # Desmontar NFS
mkfs.ext4 /dev/mmcblk0p1  # Formatear un sistema de archivos embebido
```

## 🛠 Trabajo con BusyBox
```bash
busybox                   # Ver herramientas disponibles en BusyBox
ln -s /bin/busybox ls     # Crear un enlace simbólico para usar ls desde BusyBox
