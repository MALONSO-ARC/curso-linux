## Compilación y personalización básica de U-Boot

### Objetivo
Aprender a compilar U-Boot para BeagleBone Black y modificar variables de entorno básicas.

### Pasos

1. **Clonar el repositorio oficial de U-Boot**:
```bash
git clone https://openbeagle.org/beagleboard/u-boot.git
cd u-boot
```

2. **Configurar para BeagleBone Black**:
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x_evm_defconfig
```

3. **Compilar U-Boot**:
```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j$(nproc)
```

4. **Editar variables de entorno en la consola de U-Boot**:
```bash
setenv bootdelay 5
setenv bootcmd 'echo "Bienvenido al curso de Linux Embebido"'
saveenv
```

5. **Verifica con `printenv` y `print` que se hayan guardado correctamente**:
```bash
printenv
print <variable>
```

