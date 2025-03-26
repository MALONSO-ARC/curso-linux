---

# :floppy_disk: Práctica: Compilación y Carga de un Módulo Sencillo en Ubuntu

## :dart: Objetivo

Compilar un módulo del kernel muy simple en una máquina Ubuntu, cargarlo con `insmod` y verificar las trazas generadas mediante `printk` usando `dmesg`.

---

## Requisitos

- Sistema Ubuntu (recomendado: Ubuntu 22.04)
- Paquetes de desarrollo del kernel

Instalación de dependencias:
```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```

---

## 1. Crear los archivos del módulo

### Estructura del proyecto
```
hello_kernel/
├── Makefile
└── hello_kernel.c
```
[Makefile](https://gitlab.com/codingalchemy/cursos-imagina-group/linux-embebido/-/raw/main/assets/Makefile) 

[hello_kernel.c](https://gitlab.com/codingalchemy/cursos-imagina-group/linux-embebido/-/raw/main/assets/hello_kernel.c)

Para crear la estructura de carpetas ejecutar:

```bash
mkdir hello_kernel
cd hello_kernel
cp <path-to-Makefile>/Makefile ./
cp <path-to-Makefile>/hello_kernel.c ./
``` 


## 2. Compilar el módulo

```bash
make
```

Esto generará un archivo `hello_kernel.ko`.

---

## 3. Cargar el módulo y ver trazas

### Cargar el módulo
```bash
sudo insmod hello_kernel.ko
```

### Verificar trazas
```bash
dmesg | tail -n 10
```

Deberías ver:
```
[HelloWorld] Módulo cargado: ¡Hola, mundo desde el kernel!
```

### Descargar el módulo
```bash
sudo rmmod hello_kernel
```

Y nuevamente:
```bash
dmesg | tail -n 10
```
Deberías ver:
```
[HelloWorld] Módulo descargado: Adiós, mundo.
```

---


