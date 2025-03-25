## Agregar un comando personalizado a U-Boot

### Objetivo
Modificar el código de U-Boot para incluir un nuevo comando que imprima un mensaje en la consola.

### Pasos

1. **Crear el archivo `cmd_saludo.c` dentro del directorio `cmd/`**:
```c
#include <common.h>

static int do_saludo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
    printf("\n¡Hola desde U-Boot modificado!\n");
    return 0;
}

U_BOOT_CMD(
    saludo, 1, 0, do_saludo,
    "Imprime un saludo",
    "Uso: saludo - muestra un mensaje de bienvenida"
);
```

2. **Modificar `cmd/Makefile` para incluir el nuevo archivo**:
Agrega la línea:
```makefile
obj-y += cmd_saludo.o
```

3. **Agregar una entrada en `cmd/Kconfig` si se desea control por configuración (opcional)**.

4. **Recompilar U-Boot**:
```bash
make CROSS_COMPILE=arm-linux-gnueabihf- -j$(nproc)
```

5. **Probar en consola de U-Boot**:
```bash
=> saludo
¡Hola desde U-Boot modificado!
```