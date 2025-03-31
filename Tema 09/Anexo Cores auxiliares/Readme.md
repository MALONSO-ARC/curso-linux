## Manejo de Cores Auxiliares (Cortex-M4/M7) en SoCs i.MX8 de Toradex

Los SoCs de la familia i.MX8, como los utilizados en las placas de Toradex (Colibri iMX8X, Apalis iMX8, entre otros), integran cores Cortex-A y Cortex-M que pueden colaborar en sistemas heterogéneos. Esta sección describe cómo manejar estos cores auxiliares desde U-Boot y Linux, y cómo establecer comunicación entre ellos usando `rpmsg`.

[Heterogeneous Multi-core Processing (HMP)](https://developer.toradex.com/software/cortex-m/)

---

### Arranque del Core Auxiliar desde U-Boot (`bootaux`)

#### Habilitación

1. Configurar U-Boot con `menuconfig`:

```bash
make colibri-imx8x_defconfig
make menuconfig
```

Activar:

```
-> ARM architectrue
   -> [*] Support boot auxiliary core
```

2. Verificar que el archivo `arch/arm/imx_bootaux.c` esté incluido en la compilación. Este contiene la implementación del comando `bootaux` para plataformas i.MX.

**Nota:** Aunque este archivo está presente en el árbol de U-Boot para ARM, su inclusión y compilación dependen de que estén definidas correctamente las macros y configuraciones del SoC en uso, como `CONFIG_ARCH_MX8`, `CONFIG_IMX_BOOTAUX`, y que el Device Tree incluya la información necesaria sobre el core auxiliar (por ejemplo, nodos SRC y CCM).

#### Uso

```bash
fatload mmc 0:1 0x80000000 m4_firmware.bin
bootaux 0x80000000
```

**Consideraciones:**
- Asegurar que el firmware esté enlazado a la dirección de ejecución correcta (TCM o RAM).
- Evitar usar `bootaux` si luego se usará `remoteproc` desde Linux.
- En algunos SoCs, el core M4 se arranca automáticamente si detecta firmware válido en TCM al salir de reset.

---

### Gestión desde Linux con `remoteproc`

#### Habilitación

Activar en el kernel:

```bash
CONFIG_REMOTEPROC=y
CONFIG_IMX_REMOTEPROC=y
CONFIG_RPMSG=y
```

#### Device Tree

```dts
&m4_rproc {
    compatible = "fsl,imx8qxp-m4-rproc";
    firmware-name = "imx8qxp_m4_example.elf";
    memory-region = <&vdev0vring &vdev1vring &rpmsg_reserved>;
};

reserved-memory {
    rpmsg_reserved: rpmsg@b8000000 {
        reg = <0xb8000000 0x80000>;
        no-map;
    };
};
```

#### Control del Core

```bash
echo start > /sys/class/remoteproc/remoteproc0/state
echo stop > /sys/class/remoteproc/remoteproc0/state
```

El firmware debe estar en `/lib/firmware/imx8qxp_m4_example.elf`

#### API de `remoteproc` en el Kernel

El subsistema `remoteproc` en el kernel de Linux permite manejar cores remotos a través de una API estandarizada. La estructura clave es `struct rproc_ops`, donde cada plataforma define funciones como:

```c
struct rproc_ops {
    int (*start)(struct rproc *);
    int (*stop)(struct rproc *);
    int (*load)(struct rproc *, const struct firmware *);
    void (*kick)(struct rproc *);
    ...
};
```

Para plataformas i.MX8, el archivo `drivers/remoteproc/imx_rproc.c` implementa esta API.

Si estás portando a un nuevo SoC, puede ser necesario adaptar esta estructura o crear un nuevo driver que implemente correctamente la interfaz para el manejo del core auxiliar.

---

### Comunicación entre Cores con `rpmsg`

Una vez que el Cortex-M arranca vía `remoteproc`, puede establecer comunicación con el Cortex-A usando RPMsg (Remote Processor Messaging).

#### Requisitos en el Firmware del M4

El firmware debe incluir una `resource_table` en la sección `.resource_table` del ELF. Puede utilizarse el framework de NXP (CMSIS + RPMsg-Lite).

#### Carga del Módulo en Linux

```bash
modprobe rpmsg_char
modprobe rpmsg_tty
```

Al arrancar el firmware, el kernel crea dispositivos virtuales:

```bash
/dev/rpmsg_ctrl0
/dev/ttyRPMSG0
```

#### Comunicación

Desde Linux (ejemplo con `ttyRPMSG0`):

```bash
echo "Hola desde Linux" > /dev/ttyRPMSG0
cat /dev/ttyRPMSG0
```

Desde el M4, el firmware debe usar la API de `rpmsg_lite_send()` y `rpmsg_lite_receive()` para intercambiar mensajes.

#### Aplicaciones

- RPC (llamadas a funciones remotas entre cores)
- Transmisión de datos en tiempo real (sensores, control)
- Comando y control desde Linux hacia el firmware embebido

---

### Recomendaciones Finales

- Preferir `remoteproc` para producción por su trazabilidad y soporte de recuperación.
- Verificar siempre las zonas de memoria y clocks del core auxiliar.
- Usar `rpmsg` para una arquitectura heterogénea robusta y bien desacoplada.

---

¿Dudas? Consultar el BSP de Toradex y la documentación de NXP sobre i.MX8 y FreeRTOS para Cortex-M.

