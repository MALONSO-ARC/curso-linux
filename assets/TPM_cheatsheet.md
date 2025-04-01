# 🛡️ Uso de TPM en U-Boot y Linux Embebido

## 1. Habilitación del TPM en U-Boot

### Requisitos
- U-Boot v2021.10 o superior
- Chip TPM 2.0 físico (SPI/I2C) o emulado (swtpm)
- Soporte en el device tree (si aplica)

### Configuración en U-Boot (Kconfig)
Activar las siguientes opciones en el `defconfig` de tu placa:

```text
CONFIG_TPM=y
CONFIG_TPM_V2=y
CONFIG_CMD_TPM=y
CONFIG_TPM_TIS_SPI=y      # Para TPM por SPI
CONFIG_TPM_TIS_I2C=y       # Para TPM por I2C
```

Asegura también que el soporte para comandos hash esté habilitado:
```text
CONFIG_CMD_HASH=y
CONFIG_HASH=y
CONFIG_SHA256=y
```

### Chips TPM soportados en U-Boot
| Fabricante     | Modelo           | Interface | Notas |
|----------------|------------------|-----------|-------|
| Infineon       | SLB 9670         | SPI       | Muy común en RPi y x86 |
| STMicro        | ST33TPHF2ESPI    | SPI       | Compatible con Linux |
| Nuvoton        | NPCT75x          | I2C       | TPM 2.0 I2C |
| Emulador       | swtpm            | TCP/Unix  | Para pruebas con QEMU |

## 2. Manejo del TPM desde Linux

### Acceso al TPM
- Interface de kernel: `/dev/tpm0` (v1.2/v2.0) o `/dev/tpmrm0` (TPM 2.0 con resource manager)
- Módulos: `tpm_tis`, `tpm_crb`, `tpm_tis_spi`, `tpm_i2c_infineon`, etc.

### Instalación de herramientas:
```bash
sudo apt install tpm2-tools tpm2-abrmd
```

### Comandos comunes (tpm2-tools)
| Comando                        | Descripción |
|-------------------------------|-------------|
| `tpm2_getrandom 16`           | Generar 16 bytes aleatorios |
| `tpm2_pcrread`                | Leer todos los PCR |
| `tpm2_pcrextend <idx>:sha256=<hash>` | Extender PCR con hash |
| `tpm2_nvdefine` / `nvwrite`  | Definir / escribir espacio NV |
| `tpm2_createprimary`          | Crear clave primaria |
| `tpm2_create` / `tpm2_unseal` | Crear y usar clave sellada |

## 3. 📊 Registros PCR (Platform Configuration Registers)

Los **PCR** son registros especiales dentro del TPM utilizados para registrar medidas de integridad de componentes críticos del sistema (como bootloaders o kernels).

- Son **de solo escritura**: no se puede sobrescribir directamente.
- Se usan en el concepto de **Chain of Trust**.
- Cada extensión a un PCR genera:
  ```
  PCR_new = SHA256(PCR_old || new_hash)
  ```

### Usos de los PCR
- Almacenar medidas del binario de U-Boot, kernel, etc.
- Verificación de integridad desde Linux.
- Condicionar el acceso a claves seguras en el TPM.

### Diagrama: Flujo de medida con PCR

```text
+----------------+        +--------------------+        +-----------------------+
| Binario (ej:   | hash   | SHA256(bloque mem) | extend | PCR0 = SHA(PCR0 || H) |
| u-boot.bin)    +------->+ = H                 +------->+ Registro interno TPM  |
+----------------+        +--------------------+        +-----------------------+
```

## 4. Usar TPM para verificar el binario de U-Boot

### Objetivo
Verificar la integridad de U-Boot desde U-Boot mismo usando TPM y su registro PCR.

### Flujo de trabajo
1. **Calcular hash SHA256 del binario U-Boot:**
   ```bash
   sha256sum u-boot.bin > uboot.sha256
   ```
2. **Cargar binario en memoria desde U-Boot:**
   ```bash
   load mmc 0:1 ${loadaddr} u-boot.bin
   setenv filesize <tamaño>
   ```
3. **Calcular hash desde U-Boot:**
   ```bash
   hash sha256 ${loadaddr} ${filesize} hashval
   ```
4. **Extender PCR con ese hash:**
   ```bash
   tpm pcr_extend 0 $hashval
   ```
5. **Verificar desde Linux:**
   ```bash
   tpm2_pcrread sha256:0
   # Comparar con el hash esperado de U-Boot
   ```

### Alternativa: Usar clave sellada a PCRs
- Crear clave sellada en Linux con `tpm2_create` ligada a PCR 0
- Solo se podrá desencriptar si el valor del PCR coincide (es decir, si U-Boot es el esperado)


