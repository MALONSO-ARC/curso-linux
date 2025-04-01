## 💪 Ejercicio: Introducción a SELinux en Ubuntu 22.04

### 🎯 Objetivo
Configurar SELinux en una VM con Ubuntu 22.04, verificar su estado, aplicar políticas y observar su comportamiento frente a accesos no permitidos.

---

### 📦 Paso 1: Instalar SELinux

```bash
sudo apt update
sudo apt install selinux selinux-basics selinux-policy-default auditd -y
```

---

### ⚙️ Paso 2: Activar SELinux

```bash
sudo selinux-activate
```

> Esto configurará SELinux en **modo permisivo** (registra violaciones pero no bloquea).

Reinicia la máquina:
```bash
sudo reboot
```

---

### 🔍 Paso 3: Verificar el estado de SELinux

```bash
getenforce
# Debería mostrar: Permissive

sestatus
# Muestra estado, políticas, modo actual y tipo de política.
```

---

### 🔧 Paso 4: Cambiar a modo Enforcing

```bash
sudo setenforce 1
getenforce
# Ahora debería decir: Enforcing
```

Para hacerlo permanente:

```bash
sudo nano /etc/selinux/config
# Cambia: SELINUX=permissive  ->  SELINUX=enforcing
```

---

### 📁 Paso 5: Crear un escenario de prueba

Creamos un script simple y le cambiamos el contexto SELinux para simular un acceso no autorizado.

```bash
mkdir ~/selinux-test
cd ~/selinux-test
echo -e "#!/bin/bash\necho 'Hola desde script'" > test.sh
chmod +x test.sh
```

Ver el contexto actual:
```bash
ls -Z
```

Cambiar el contexto de manera incorrecta (simulando una falla):
```bash
sudo chcon -t httpd_sys_content_t test.sh
```

Intentar ejecutar el script:
```bash
./test.sh
# Verifica si hay errores o si fue bloqueado por SELinux
```

---

### 📜 Paso 6: Revisar los logs

```bash
sudo journalctl -xe | grep AVC
sudo ausearch -m avc -ts recent
```

Esto mostrará los eventos bloqueados por SELinux.

---

### ✅ Paso 7: Restaurar el contexto correcto

```bash
sudo restorecon -v test.sh
ls -Z
./test.sh
# Ahora debe ejecutarse sin problemas
```

---

### 🧐 Preguntas para reflexionar

1. ¿Qué ventajas tiene SELinux frente a un modelo clásico de permisos?
2. ¿Cuál es la diferencia entre los modos permissive y enforcing?
3. ¿Qué tipos de políticas existen en SELinux?
4. ¿Cómo podrías aplicar SELinux en un sistema embebido que use Linux?

