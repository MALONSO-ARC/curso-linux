---

# :hammer: Ejercicio: Añadir un Driver Personalizado al Kernel de Linux

## :dart: Objetivo

Integrar un driver personalizado al árbol de fuentes del kernel de Linux para que pueda activarse desde `menuconfig` y compilarse como parte del sistema o como módulo.

---

## :file_folder: Estructura del Driver

Supongamos que vamos a añadir un driver llamado `hello.c`.

Ubicación sugerida:

```bash
linux/drivers/char/
```

Archivos:
- `hello.c` : El código fuente del driver
- `Makefile` : Instrucciones para compilar el driver
- `Kconfig` : Entrada de configuración para `menuconfig`

---

## :wrench: Pasos para Integración

### 1. Crear carpeta(opcional) y archivos

```bash
cd drivers/char
cp hello.c ./
```

### 2. Editar Makefile y Kconfig 

#### `Makefile`
```make
obj-$(obj-$(CONFIG_HELLO_DRIVER) += hello.o
) += hello.o
```

#### `Kconfig`
```make
config HELLO_DRIVER 
    tristate "Hello driver"
    default y
    help
      Este driver saludo por dmesg
```


## Compilación

### 1. Configurar el kernel

```bash
make menuconfig
# Navega a "Device Drivers" y activa "Hello driver"
```

### 2. Compilar

```bash
make -j$(nproc) modules
```

Si el driver fue marcado como módulo (`m`), el archivo `hello.ko` estará disponible en `drivers/char/`.

---

## Prueba del módulo

```bash
insmod hello.ko
# o
modprobe hello
```

Verifica los mensajes con:
```bash
dmesg | grep foo
```

---

## :brain: Preguntas de reflexión

1. ¿Cuál es la ventaja de integrar un driver al árbol del kernel?
2. ¿Cuándo deberías compilarlo como módulo y cuándo como built-in?
3. ¿Cómo puedes automatizar la inclusión de drivers propios en un build system como Yocto o Buildroot?

---

