# Yocto Cheatsheet

Una hoja de referencia rápida para trabajar con el sistema de construcción Yocto Project.

## 🧱 Comandos Básicos

| Acción | Comando |
|--------|---------|
| Ver recetas disponibles | `bitbake -s` |
| Compilar una receta | `bitbake <receta>` |
| Limpiar una receta | `bitbake -c clean <receta>` |
| Entrar al entorno de compilación | `bitbake -c devshell <receta>` |
| Mostrar variables de una receta | `bitbake -e <receta>` |
| Ver tareas disponibles de una receta | `bitbake -c listtasks <receta>` |

---

## 🗂️ Capas (Layers)

| Acción | Comando |
|--------|---------|
| Ver capas activas | `bitbake-layers show-layers` |
| Ver recetas por capa | `bitbake-layers show-recipes` |
| Ver `.bbappend` activos | `bitbake-layers show-appends` |
| Añadir una capa | `bitbake-layers add-layer <ruta>` |

---

## 📦 Imágenes y Paquetes

| Acción | Comando |
|--------|---------|
| Construir una imagen | `bitbake core-image-minimal` |
| Ver qué recetas se ejecutan | `bitbake core-image-minimal -g` (genera `pn-buildlist`) |
| Añadir paquetes a una imagen | `IMAGE_INSTALL += "<paquete>"` |
| Quitar paquetes de una imagen | `IMAGE_INSTALL:remove = "<paquete>"` |
| Evitar recomendaciones | `BAD_RECOMMENDATIONS += "<paquete>"` |

---

## 🔧 Configuración

| Archivo | Ubicación | Propósito |
|---------|-----------|-----------|
| `local.conf` | `build/conf/` | Configuración del entorno local |
| `bblayers.conf` | `build/conf/` | Define las capas activas |
| `.bb` | `recipes-*/` | Receta principal de un paquete |
| `.bbappend` | `recipes-*/` | Extensión/modificación de una receta |

---

## 🛠️ Recetas - Variables Clave

| Variable | Propósito |
|----------|-----------|
| `DESCRIPTION` | Descripción del paquete |
| `LICENSE` | Licencia del software |
| `SRC_URI` | Dónde obtener el código fuente |
| `S` | Directorio de trabajo con el código fuente |
| `do_compile` | Cómo compilar |
| `do_install` | Cómo instalar en el `rootfs` |

---

## 📁 Directorios Importantes

| Ruta | Contenido |
|------|-----------|
| `tmp/deploy/images/` | Imágenes generadas y binarios |
| `tmp/work/` | Archivos intermedios por receta y arquitectura |
| `meta*/` | Capas (layers) con recetas, configuraciones, clases |

---

## 🧪 Comandos Útiles de Debug

```bash
bitbake -e <receta>         # Ver entorno de variables
bitbake <receta> -c devshell  # Entrar al entorno de build
bitbake -c listtasks <receta> # Ver tareas disponibles
```

---

## 🧠 Tips

- Usa `DISTRO_FEATURES` para incluir/excluir soporte a funcionalidades del sistema:
```bash
DISTRO_FEATURES:remove = "x11 nfs bluetooth"
```
- Usa `EXTRA_IMAGE_FEATURES += "debug-tweaks"` para permitir acceso por root sin contraseña.
- Usa `INHERIT += "rm_work"` para reducir espacio después de la compilación.

---

## 🧩 Sitios útiles

- https://www.yoctoproject.org/
- https://layers.openembedded.org/layerindex/
- https://docs.yoctoproject.org/

---

