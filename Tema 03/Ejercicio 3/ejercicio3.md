## 💻 Ejercicio 3: Administración de permisos y control de usuarios

### Objetivo
Practicar la creación de usuarios, asignación de grupos y modificación de permisos de archivos.

### Instrucciones
1. Crear un usuario llamado `embebido` y un grupo llamado `dev`.
2. Añadir el usuario al grupo:
   ```bash
   sudo usermod -aG dev embebido
   ```
3. Crear el archivo `/home/embebido/info.txt` y escribir algo dentro.
4. Asignar como propietario al usuario `embebido` y grupo `dev`:
   ```bash
   sudo chown embebido:dev /home/embebido/info.txt
   ```
5. Cambiar los permisos para que solo el propietario pueda leer y escribir:
   ```bash
   chmod 600 /home/embebido/info.txt
   ```
6. Verifica los permisos con `ls -l` y documenta el resultado.

### Entregables
- Captura del comando `ls -l /home/embebido/info.txt`.
- Comandos utilizados.
- Explicación del significado de los permisos `-rw-------`.

---

**Fin de los ejercicios del Tema 3.** Estos ejercicios consolidan el uso de comandos esenciales de Linux, scripting básico en Bash y operaciones comunes de administración de sistemas.

