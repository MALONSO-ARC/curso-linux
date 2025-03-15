# Tema 7: Sistema de Archivos Root (RootFS)

## 7.1 ¿Qué es RootFS y por qué es importante?
El RootFS (Root File System) es el sistema de archivos raíz en un sistema Linux embebido. Contiene todos los archivos y directorios esenciales necesarios para el arranque y la operación del sistema. Su correcto diseño y configuración afectan directamente el rendimiento, la seguridad y la capacidad de actualización del sistema embebido.

## 7.2 Tipos de Sistemas de Archivos en Linux Embebido
Existen varios tipos de sistemas de archivos utilizados en Linux embebido, cada uno con características específicas:
- **ext4**: Común en sistemas de almacenamiento estándar, soporta journaling.
- **JFFS2**: Diseñado para memorias flash NOR, adecuado para sistemas con escrituras frecuentes.
- **UBIFS**: Optimizado para memorias flash NAND, ofrece mejor rendimiento y compresión.
- **SquashFS**: Sistema de archivos comprimido de solo lectura, ideal para sistemas con almacenamiento limitado.

## 7.3 Uso de Initramfs y RootFS Personalizados
- **Initramfs**: Sistema de archivos inicial cargado en RAM que facilita el arranque.
- **RootFS Personalizado**: Adaptado a las necesidades del sistema embebido, minimizando tamaño y optimizando rendimiento.

## 7.4 Estructura de Directorios en Linux Embebido
Los sistemas Linux embebidos siguen una estructura de directorios similar a las distribuciones estándar, pero optimizada para entornos de recursos limitados:
- `/bin`: Binarios esenciales.
- `/lib`: Bibliotecas compartidas.
- `/dev`: Archivos de dispositivos.
- `/sys`: Información sobre el sistema y los dispositivos.

## 7.5 Creación de RootFS con BusyBox
BusyBox es una colección de utilidades optimizadas para sistemas embebidos. Pasos para crear un RootFS con BusyBox:
1. Configuración y compilación de BusyBox.
2. Instalación en un directorio base.
3. Creación de los directorios esenciales.
4. Generación de imágenes del sistema de archivos.

## 7.6 Métodos de Carga del RootFS en Sistemas Embebidos
Diferentes métodos para cargar el RootFS en un sistema embebido:
- **Montaje desde memoria flash**.
- **Uso de Initramfs en RAM**.
- **Carga desde red usando NFS**.
- **Montaje desde un almacenamiento externo (USB, SD)**.

## 7.7 Configuración de Permisos y Seguridad en RootFS
- Definición de permisos adecuados para archivos y directorios.
- Uso de herramientas como `chown` y `chmod`.
- Aplicación de políticas de seguridad como SELinux o AppArmor.

## 7.8 Soporte para Montaje Remoto con NFS
NFS (Network File System) permite montar un sistema de archivos remoto en un sistema embebido. Pasos básicos:
1. Configuración del servidor NFS.
2. Configuración del cliente NFS en el dispositivo embebido.
3. Montaje del sistema de archivos remoto.

## 7.9 Compresión y Optimización del RootFS
Para reducir el tamaño y mejorar el rendimiento:
- Uso de SquashFS para un RootFS de solo lectura comprimido.
- Eliminación de archivos innecesarios.
- Uso de herramientas como `mksquashfs` y `zstd` para compresión avanzada.

## 7.10 Debugging de Problemas en RootFS
Técnicas para solucionar problemas relacionados con RootFS:
- Análisis de logs del sistema (`dmesg`, `/var/log/messages`).
- Uso de `strace` y `lsof` para monitoreo de acceso a archivos.
- Verificación de integridad con `fsck` y `debugfs`.

## 7.11 Conclusión
El correcto diseño y configuración del RootFS en sistemas embebidos es clave para garantizar eficiencia, seguridad y funcionalidad. El conocimiento de los diferentes sistemas de archivos y herramientas disponibles permite optimizar el rendimiento y reducir el consumo de recursos.
