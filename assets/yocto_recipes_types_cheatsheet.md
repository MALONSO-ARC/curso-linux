# 📟 Yocto Recipe Cheatsheet

Guía rápida con ejemplos de los principales tipos de recetas que se pueden crear en Yocto Project.

---

## 📆 1. Receta de Aplicación (ej. binario, app propia)

**Propósito**: Compilar e instalar una aplicación del código fuente.

```bitbake
DESCRIPTION = "Aplicación ejemplo"
LICENSE = "MIT"
SRC_URI = "git://git.miempresa.com/miapp.git;branch=main"
S = "${WORKDIR}/git"

inherit cmake

do_install() {
    install -d ${D}${bindir}
    install -m 0755 miapp ${D}${bindir}/miapp
}
```

---

## ⚙️ 2. Receta de Configuración (ficheros en /etc)

**Propósito**: Instalar ficheros de configuración en el sistema.

```bitbake
DESCRIPTION = "Fichero de configuración para MyApp"
LICENSE = "CLOSED"
SRC_URI += "file://myapp.conf"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${sysconfdir}/myapp
    install -m 0644 ${WORKDIR}/myapp.conf ${D}${sysconfdir}/myapp/myapp.conf
}
```

---

## 👤 3. Receta para Añadir Usuarios

**Propósito**: Crear usuarios en el sistema con contraseña y grupos.

```bitbake
SUMMARY = "Crear usuario user1"
LICENSE = "MIT"

inherit useradd

USERADD_PACKAGES = "${PN}"
USERADD_PARAM:${PN} = "-u 1001 -G users -p '\$6\$salt\$hash' user1"

do_install[noexec] = "1"
```

*Reemplazar `\$6\$salt\$hash` por un hash generado con `openssl passwd -6`*

---

## 📂 4. Receta para Archivos Estáticos (script, datos)

**Propósito**: Incluir scripts, documentos, librerías, etc.

```bitbake
DESCRIPTION = "Instala scripts auxiliares"
LICENSE = "MIT"
SRC_URI += "file://run_analysis.sh"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/run_analysis.sh ${D}${bindir}/run_analysis.sh
}
```

---

## 🧱 5. Receta de Librería Compartida

**Propósito**: Compilar e instalar una librería (.so)

```bitbake
DESCRIPTION = "Librería compartida ejemplo"
LICENSE = "MIT"
SRC_URI = "file://libfoo.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} -fPIC -shared libfoo.c -o libfoo.so
}

do_install() {
    install -d ${D}${libdir}
    install -m 0644 libfoo.so ${D}${libdir}/libfoo.so
}
```

---

## 🔧 6. Receta de Patch / Parchear otro paquete

**Propósito**: Aplicar un parche sobre una receta existente

```bitbake
DESCRIPTION = "Parche para busybox"
LICENSE = "MIT"
SRC_URI += "file://fix-busybox.patch"
```

*Coloca la receta en una capa con mayor prioridad y usa el mismo nombre que la original o un bbappend.*

---

## 🪩 7. bbappend: Añadir funcionalidad a recetas existentes

**Ejemplo**: Añadir un fichero de configuración a opencv-app

```bitbake
# opencv-app.bbappend
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://my_app.conf"

do_install:append() {
    install -d ${D}${sysconfdir}/opencv
    install -m 0644 ${WORKDIR}/my_app.conf ${D}${sysconfdir}/opencv/my_app.conf
}
```

---

## 🛠️ 8. Receta para definir variables de entorno en U-Boot

**Propósito**: Añadir o modificar variables de entorno persistentes en U-Boot.

```bitbake
DESCRIPTION = "Variables de entorno personalizadas para U-Boot"
LICENSE = "MIT"

SRC_URI += "file://custom_env.txt"

do_install:append() {
    install -d ${D}${sysconfdir}/u-boot
    install -m 0644 ${WORKDIR}/custom_env.txt ${D}${sysconfdir}/u-boot/custom_env.txt
}

FILES:${PN} += "${sysconfdir}/u-boot/custom_env.txt"
```

*Contenido ejemplo de `custom_env.txt`:*
```
bootdelay=3
bootcmd=run boot_linux
ipaddr=192.168.1.100
```

Estas variables pueden cargarse desde el sistema o mediante script en runtime usando `fw_setenv`.
