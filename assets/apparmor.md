# AppArmor Cheatsheet

## ✨ Características clave de AppArmor

- **MAC (Mandatory Access Control)**: Control de acceso obligatorio basado en perfiles.
- **Perfiles por aplicación**: Define qué recursos puede usar cada programa.
- **Modos de funcionamiento**:
  - *Complain (modo aprendizaje)*: Registra violaciones sin bloquear.
  - *Enforce (modo estricto)*: Bloquea según las reglas.
- **Perfiles legibles por humanos**: Fáciles de escribir y mantener.
- **Soporte para plantillas y variables**: Para simplificar perfiles complejos.
- **Integración con systemd y snap**: Amplio soporte en Ubuntu y derivados.

---

## 📁 Archivos y rutas importantes

| Ruta | Descripción |
|------|-------------|
| `/etc/apparmor/` | Directorio principal de configuración. |
| `/etc/apparmor.d/` | Contiene perfiles individuales de aplicaciones. |
| `/etc/apparmor.d/disable/` | Perfiles desactivados. |
| `/var/log/syslog` o `/var/log/audit/audit.log` | Logs de AppArmor. |
| `/sys/kernel/security/apparmor/` | Interfaz del kernel. |
| `/proc/<pid>/attr/current` | Estado de confinamiento de un proceso. |

---

## 🔄 Comandos esenciales

```bash
# Ver el estado de AppArmor
auditctl -s
sudo aa-status

# Cargar y descargar perfiles
sudo apparmor_parser -r <perfil>   # Recargar
sudo apparmor_parser -R <perfil>   # Descargar

# Activar o desactivar perfiles
sudo aa-enforce <perfil>     # Activar en modo enforce
sudo aa-complain <perfil>    # Activar en modo complain
sudo aa-disable <perfil>     # Desactivar

# Generar y modificar perfiles
sudo aa-genprof <programa>
sudo aa-logprof               # Analiza logs y sugiere reglas

# Listar perfiles activos
sudo apparmor_status
```

---

## 🔎 Sintaxis básica de perfiles

```text
#include <tunables/global>

/profile/path {
  # Modos de acceso
  /ruta/de/archivo rw,
  /bin/* ix,
  /tmp/** rwk,
  capability net_bind_service,
  network inet stream,
}
```

- `r`, `w`, `x`, `k`: leer, escribir, ejecutar, bloquear.
- `ix`: ejecutar sin heredar perfil.
- `cx`: ejecutar con cambio de perfil.

---

## 🔧 Tips y buenas prácticas

- Comienza en modo *complain* para aprender sin bloquear.
- Usa `aa-logprof` frecuentemente para ajustar reglas.
- Divide perfiles en fragmentos usando `#include`.
- Usa `auditd` para logs detallados.
- Revisa `man apparmor.d` para más ejemplos de sintaxis.

---

## ℹ️ Recursos

- https://gitlab.com/apparmor/apparmor/wikis/home
- https://wiki.ubuntu.com/AppArmor
- `man apparmor`, `man apparmor.d`, `man aa-status`

---

> 💡 Ideal para proteger servicios como nginx, mysql, sshd o scripts personalizados.

