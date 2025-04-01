# 🛡️ Lienzo de Práctica: Activar AppArmor y Crear un Perfil de Restricción

## 🎯 Objetivo
Aplicar políticas de control de acceso mediante AppArmor, generando un perfil personalizado para restringir el acceso a archivos sensibles desde un script.

---

## 🧱 Requisitos previos

- Sistema Linux con AppArmor instalado y activo.
- Privilegios de superusuario (`sudo`).
- Usuario con permisos sobre `/home/linux`.

---

## 📁 Estructura de archivos

| Ruta                                  | Descripción                                 |
|---------------------------------------|---------------------------------------------|
| `/home/linux/archivo.txt`           | Archivo a proteger                           |
| `/home/linux/prueba.sh`             | Script de prueba que accederá a dos archivos |
| `/etc/apparmor.d/home.usr.prueba.sh`  | Perfil generado por `aa-genprof`             |

---

## 🧪 Pasos detallados

### 1. Verificar que AppArmor está activo
```bash
sudo aa-status
```
**Resultado esperado:** Lista de perfiles activos y modo de aplicación (`enforce`, `complain`).

---

### 2. Crear el archivo sensible
```bash
echo "Contenido sensible" > /home/linux/archivo.txt
```

---

### 3. Crear el script de prueba
```bash
cat <<EOF > /home/linux/prueba.sh
#!/bin/bash
cat /etc/passwd
cat /home/linux/archivo.txt
EOF

chmod +x /home/linux/prueba.sh
```

---

### 4. Generar perfil AppArmor con `aa-genprof`
```bash
sudo aa-genprof /home/linux/prueba.sh
```

Cuando se te indique:

1. Ejecuta el script en otra terminal:
   ```bash
   ./prueba.sh
   ```

2. En el asistente de AppArmor:
   - **Permite** el acceso a `/etc/passwd`.
   - **Deniega** el acceso a `/home/linux/archivo.txt`.
   - Guarda y finaliza el perfil.

---

### 5. Activar el perfil en modo "enforce"
```bash
sudo aa-enforce /etc/apparmor.d/home.linux.prueba.sh
```

Ejecuta de nuevo el script:
```bash
./prueba.sh
```

---

## ✅ Resultados esperados

**Salida del script:**
```
[contenido de /etc/passwd]
cat: /home/linux/archivo.txt: Permission denied
```

---

## 🔍 Ver logs de denegación

```bash
journalctl -xe | grep DENIED
```

**Resultado esperado:** Mensajes del kernel como:
```
inux-embebido audit[3030]: AVC apparmor="DENIED"operation="open" ...
```

---

## 🧹 Limpieza (opcional)

```bash
sudo aa-disable /etc/apparmor.d/home.usr.prueba.sh
rm /home/linux/prueba.sh
rm /home/linux/archivo.txt
```


