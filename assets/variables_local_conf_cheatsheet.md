---

# Yocto Cheatsheet: Variables clave en `conf/local.conf`

Esta hoja resume las variables más comunes y útiles que pueden configurarse en el archivo `conf/local.conf` de Yocto, basadas en la documentación oficial del proyecto.

---

## 🧩 Variables fundamentales

| Variable             | Descripción                                                                 |
|----------------------|-----------------------------------------------------------------------------|
| `MACHINE`            | Define la máquina objetivo (ej. `qemuarm`, `beaglebone`, `raspberrypi4`)    |
| `DISTRO`             | Define la distribución Yocto activa (ej. `poky`, `poky-tiny`)               |
| `PACKAGE_CLASSES`    | Define el backend de paquetes (`package_ipk`, `package_rpm`, `package_deb`) |
| `TMPDIR`             | Ruta del directorio temporal (`tmp/`)                                       |
| `DL_DIR`             | Ruta para almacenar descargas compartidas                                    |
| `SSTATE_DIR`         | Ruta del caché compartido de estado (`sstate-cache`)                        |
| `BB_NUMBER_THREADS`  | Núm. de hilos usados por BitBake                                             |
| `PARALLEL_MAKE`      | Núm. de hilos usados por `make` durante compilación                         |

---

## ⚙️ Control del sistema de construcción

| Variable             | Descripción                                                                |
|----------------------|----------------------------------------------------------------------------|
| `BBLAYERS`           | Lista de capas habilitadas (definido en `bblayers.conf`)                   |
| `BBMASK`             | Excluye recetas por patrón de nombre                                       |
| `PREFERRED_PROVIDER_<pn>` | Define qué proveedor usar para una receta específica                   |
| `PREFERRED_VERSION_<pn>`  | Fuerza una versión específica para una receta                           |
| `INHERIT`            | Lista de clases que se heredan globalmente                                 |

---

## 🛠️ Variables para debug y desarrollo

| Variable                | Descripción                                                              |
|-------------------------|--------------------------------------------------------------------------|
| `INHERIT += "rm_work"` | Elimina archivos temporales tras la compilación                         |
| `INHERIT += "buildstats"` | Muestra estadísticas de compilación                                 |
| `INHERIT += "testimage"`  | Activa ejecución de pruebas con `bitbake -c testimage`               |
| `USER_CLASSES`          | Añade clases de usuario como `buildstats`, `image-mklibs`, `image-prelink` |

---

## 🧩 Personalización de la imagen

| Variable                     | Descripción                                                         |
|------------------------------|---------------------------------------------------------------------|
| `IMAGE_INSTALL:append`       | Añade paquetes adicionales a la imagen (`" nano htop"`)           |
| `IMAGE_FEATURES`             | Activa funcionalidades como `ssh-server-dropbear`, `debug-tweaks`  |
| `EXTRA_IMAGE_FEATURES`       | Añade características extendidas a la imagen (`tools-debug`, etc.) |
| `LICENSE_FLAGS_ACCEPTED`     | Acepta licencias especiales como `commercial`                      |
| `IMAGE_FSTYPES`              | Define los formatos de imagen generados (`ext4`, `wic`, `tar.gz`)   |
| `EXTRA_IMAGEDEPENDS`         | Añade dependencias de compilación que no se instalan en el rootfs   |

---

## 🔐 Seguridad y licencias

| Variable                    | Descripción                                                          |
|-----------------------------|----------------------------------------------------------------------|
| `LICENSE`                   | Licencia principal de una receta                                     |
| `LICENSE_FLAGS_ACCEPTED`   | Lista de licencias que el sistema debe aceptar                      |
| `NO_RECOMMENDATIONS`       | Evita instalar paquetes recomendados por otras recetas               |
| `PACKAGE_EXCLUDE`          | Lista de paquetes que deben excluirse de la imagen                   |

---

## 🧪 SDK y toolchain

| Variable                        | Descripción                                                        |
|---------------------------------|--------------------------------------------------------------------|
| `SDKMACHINE`                    | Arquitectura anfitrión del SDK (`x86_64`, `i686`, etc.)            |
| `TOOLCHAIN_HOST_TASK`           | Paquetes incluidos en el SDK del host                              |
| `TOOLCHAIN_TARGET_TASK`         | Paquetes incluidos en el SDK del target                            |
| `INHERIT += "populate_sdk"`     | Habilita la generación del SDK estándar                            |
| `INHERIT += "populate_sdk_ext"` | Habilita la generación del SDK extendido                           |

---

## 📚 Tips de uso

- Las variables pueden modificarse con los operadores `:append`, `:prepend` y `:remove`.
- Para inspeccionar el valor de una variable en tiempo de compilación:

```bash
bitbake -e <receta> | grep ^VARIABLE=
```

- Puedes crear diferentes configuraciones usando múltiples archivos `local.conf` por máquina o proyecto.

---

## 🔗 Referencia oficial

[https://docs.yoctoproject.org/ref-manual/variables.html](https://docs.yoctoproject.org/ref-manual/variables.html)

---


