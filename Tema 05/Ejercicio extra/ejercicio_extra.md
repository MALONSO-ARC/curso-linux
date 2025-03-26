### Objetivo

Compilar U-Boot con la configuración `sandbox_defconfig` para poder ejecutar U-Boot de forma local (en espacio de usuario, sin hardware real).

### Pasos

1. Clonar U-Boot:
```bash
git clone https://source.denx.de/u-boot/u-boot.git
cd u-boot
```

2. Configurar para sandbox:
```bash
make sandbox_defconfig
```

3. Compilar:
```bash
make -j$(nproc)
```

4. Ejecutar:
```bash
./u-boot
```

Deberías ver la consola de U-Boot ejecutándose en tu terminal como si fuera hardware real.

### **Nota importante**

En la máquina virtual del curso, es posible que falten dependencias necesarias para compilar `sandbox_defconfig`. Instálalas con:

```bash
sudo apt install libsdl2-dev libgnutls28-dev
```


