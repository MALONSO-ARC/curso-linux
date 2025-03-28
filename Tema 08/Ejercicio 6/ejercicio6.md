---

# Práctica: Crear una Meta-Layer para una Distro Interna de Empresa

## Objetivo

Crear una meta-layer personalizada que defina una distribución propia de la empresa, donde se puedan configurar opciones globales como exclusión de paquetes, variables de compilación, licencias aceptadas, formato de paquetes y más.

---

## Parte 1: Crear la estructura de la meta capa

1. Inicia el entorno de build:

```bash
cd poky
source oe-init-build-env
```

2. Crea la capa personalizada:

```bash
bitbake-layers create-layer ../meta-miempresa
```

3. Añade la capa al entorno de compilación:

```bash
bitbake-layers add-layer ../meta-miempresa
```

---

## Parte 2: Crear una distro personalizada

1. Crea el directorio para la configuración de la distro:

```bash
mkdir -p ../meta-miempresa/conf/distro
```

2. Crea el archivo `poky-miempresa.conf`:

```bash
touch ../meta-miempresa/conf/distro/poky-miempresa.conf
```

3. Edita el contenido:

```bitbake
require conf/distro/poky.conf

DISTRO = "poky-miempresa"
DISTRO_NAME = "Distribucion Interna Empresa"
DISTRO_VERSION = "1.0"

DISTRO_FEATURES:remove = "python3 x11"
PACKAGE_CLASSES = "package_deb"

LICENSE_FLAGS_ACCEPTED = "commercial"
PACKAGE_EXCLUDE += "python3 python3-core perl"
BAD_RECOMMENDATIONS += "python3 python3-core perl"
NO_RECOMMENDATIONS = "1"

INHERIT += "rm_work"
```

---

## Parte 3: Activar la nueva distro

1. Edita el archivo `conf/local.conf`:

```bash
DISTRO = "poky-miempresa"
```

2. (Opcional) Limpia la compilación anterior:

```bash
bitbake -c cleansstate core-image-minimal
```

3. Compila una imagen:

```bash
bitbake core-image-minimal
```

---

## Parte 4: Verifica los resultados

1. Abre el archivo `pn-buildlist` generado con:

```bash
bitbake core-image-minimal -g
cat pn-buildlist | grep python
```

Debería devolver una lista vacía (o mínima) si se han eliminado correctamente.

2. Comprueba el tipo de paquetes generados:

```bash
ls tmp/deploy/deb/
```
