# Práctica: Identificación y Eliminación de Recetas en Yocto

## Objetivo

Aprender a identificar qué recetas se van a ejecutar en una construcción de imagen de Yocto y aplicar diferentes estrategias para excluirlas o eliminarlas del sistema de archivos final.

## Parte 1: Listar las recetas activas

### 1.1. Mostrar todas las recetas disponibles

```bash
bitbake -s
```
Esto lista todas las recetas cargadas desde los layers activos.

### 1.2. Mostrar las recetas que se ejecutarán al construir una imagen

```bash
bitbake core-image-minimal -g
```
Esto generará dos archivos en el directorio actual:

- `pn-buildlist`: lista de recetas que se ejecutarán
- `task-depends.dot`: grafo de dependencias

Verifica el contenido:
```bash
cat pn-buildlist | sort
```

---

## Parte 2: Identificar recetas que no queremos incluir

Revisa la lista y elige algunas recetas innecesarias para tu proyecto. Por ejemplo:
- `packagegroup-core-tools-debug`
- `gdb`
- `alsa-utils`

Puedes usar:
```bash
grep alsa pn-buildlist
```

---

## Parte 3: Estrategias para eliminar recetas de la imagen

### 3.1. Usar `IMAGE_INSTALL:remove`

Edita tu receta de imagen personalizada (ej: `core-image-ejemplo.bb`) o `local.conf`:

```bitbake
IMAGE_INSTALL:remove = "alsa-utils gdb"
```

### 3.2. Usar `BAD_RECOMMENDATIONS`

Algunas recetas se incluyen por dependencia recomendada. Puedes excluirlas:

```bitbake
BAD_RECOMMENDATIONS += "packagegroup-core-tools-debug gdb"
```

> Esto evita que se instalen incluso si otra