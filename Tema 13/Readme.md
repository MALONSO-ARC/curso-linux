# Tema 13: Seguridad en Linux Embebido

La seguridad en sistemas embebidos es fundamental, especialmente en sectores como el IoT, automoción o industrial, donde una vulnerabilidad puede comprometer dispositivos en campo, privacidad de usuarios o incluso la seguridad física.

---

## 1. Conceptos básicos de seguridad en sistemas embebidos

Los sistemas embebidos presentan desafíos de seguridad particulares debido a sus restricciones de hardware, su ciclo de vida largo y su despliegue en entornos potencialmente hostiles.

**Principales características de seguridad:**
- **Confidencialidad:** proteger los datos del usuario.
- **Integridad:** asegurar que los datos y firmware no han sido modificados.
- **Autenticidad:** verificar la identidad de firmware, usuarios o servicios.
- **Disponibilidad:** garantizar que el sistema está accesible y operativo.

**Ejemplo:** Una cámara IP conectada a internet debe proteger las credenciales de acceso, evitar que su firmware sea modificado por terceros, y mantenerse operativa incluso ante ataques.

---

## 2. Modelado de amenazas y análisis de riesgos

### Objetivos
- Identificar los activos críticos del sistema.
- Evaluar amenazas relevantes.
- Detectar vulnerabilidades existentes.
- Estimar y priorizar los riesgos.

---

### 🔍 1. Identificación de Activos

| Tipo de Activo        | Ejemplos                                             |
|------------------------|------------------------------------------------------|
| Firmware               | Particiones de arranque, código de aplicación       |
| Credenciales           | Usuarios del sistema, claves privadas               |
| Interfaces físicas     | UART, JTAG, SPI, I2C                                |
| Interfaces de red      | Ethernet, WiFi, Bluetooth                           |
| Almacenamiento         | Configuración persistente, variables de entorno     |
| Elementos conectados   | Sensores, actuadores                                |
| Mecanismos de actualización | OTA, USB, interfaces debug                     |

---

### 2. Evaluación de Amenazas

#### Tipos comunes
- Ataques físicos (JTAG, glitching, extracción de memoria)
- Ataques remotos (MITM, escalada de privilegios)
- Ataques al firmware (inyección o reemplazo)
- Backdoors, contraseñas débiles, puertos abiertos

#### Método STRIDE

| Letra | Amenaza               | Ejemplo embebido                             |
|-------|------------------------|----------------------------------------------|
| S     | Spoofing               | Suplantación vía UART o SSH                  |
| T     | Tampering              | Modificación de firmware                     |
| R     | Repudiation            | Falta de logs de auditoría                   |
| I     | Information Disclosure | Fugas por UART o bus compartido              |
| D     | Denial of Service      | Sobrecarga de CPU o red                      |
| E     | Elevation of Privilege| Root desde servicio vulnerable               |

---

### Análisis de Vulnerabilidades

| Elemento                  | Posible Vulnerabilidad                        |
|---------------------------|-----------------------------------------------|
| Servicios de red          | Puertos abiertos, protocolos inseguros        |
| Firmware                  | Sin autenticación o cifrado                   |
| Interfaz debug            | UART/JTAG habilitado en producción            |
| Configuración del sistema | Contraseñas por defecto, permisos incorrectos |
| Drivers                   | Acceso sin restricciones                     |

**Herramientas útiles:**
- OpenVAS / Nessus → escaneo de servicios
- Binwalk, Firmware-Mod-Kit → análisis de firmware
- Lynis, Tiger → auditoría de seguridad Linux

---

### 📊 4. Estimación del Riesgo

#### Fórmula base:
Riesgo = Impacto × Probabilidad


#### Método DREAD

| Letra | Métrica         | Descripción                                      |
|-------|------------------|--------------------------------------------------|
| D     | Damage           | Daño que puede causar                           |
| R     | Reproducibility  | Facilidad para repetir el ataque                |
| E     | Exploitability   | Facilidad para explotar                         |
| A     | Affected users   | Usuarios impactados                             |
| D     | Discoverability  | Facilidad para descubrir la vulnerabilidad      |

Se asigna un valor numérico a cada métrica y se promedia.


### 5. Método TARA (Automoción y sistemas críticos)

1. Identificación de activos
2. Escenarios de amenaza
3. Factibilidad del ataque
4. Estimación del impacto
5. Clasificación del riesgo


### 📦 6. Ejemplo: Dron de Vigilancia con Visión Artificial

**Contexto:** Dron autónomo que transmite vídeo en tiempo real vía WiFi y se controla remotamente.

| Elemento                  | Detalle                                              |
|---------------------------|------------------------------------------------------|
| Activo                    | Firmware de vuelo y visión, claves de acceso         |
| Amenaza                   | Inyección de firmware modificado para redirigir vuelo|
| Vector                    | Acceso remoto a interfaz OTA no autenticada          |
| Impacto                   | Alto – pérdida de control, filtración de datos       |
| Probabilidad              | Media – requiere acceso a la red + conocimiento      |
| Riesgo estimado           | Alto                                                 |
| Contramedidas             | Secure Boot, cifrado de comunicaciones, OTA firmada |


## 3. Introducción a Secure Boot y su implementación en U-Boot

**Secure Boot** garantiza que el firmware ejecutado en el arranque es legítimo y no ha sido modificado.

### ¿Cómo funciona?
- Se almacena en el hardware una clave pública o hash verificado.
- El bootloader verifica la firma digital del siguiente componente (kernel, RootFS).
- Si la firma es válida, el sistema continúa. Si no, se detiene el arranque.

### U-Boot y Secure Boot
U-Boot admite firma de imágenes usando **FIT (Flattened Image Tree)** con soporte de verificación RSA.

**Pasos típicos:**
1. Crear claves RSA:
   ```bash
   openssl genpkey -algorithm RSA -out devkey.key -pkeyopt rsa_keygen_bits:2048
   openssl rsa -in devkey.key -pubout -out devkey.pub
   ```

2. Crear archivo `fit.its` con descripción del kernel, initrd, dtb y firmas:
   ```text
    /dts-v1/;

    / {
        description = "FIT image with kernel, FDT and signature";
        #address-cells = <1>;

        images {
            kernel@1 {
                description = "Linux Kernel";
                data = /incbin/("zImage");
                type = "kernel";
                arch = "arm";
                os = "linux";
                compression = "none";
                load = <0x80008000>;
                entry = <0x80008000>;
                hash@1 {
                    algo = "sha256";
                };
            };

            fdt@1 {
                description = "Flattened Device Tree blob";
                data = /incbin/("vexpress-v2p-ca9.dtb");
                type = "flat_dt";
                arch = "arm";
                compression = "none";
                hash@1 {
                    algo = "sha256";
                };
            };
        };

        configurations {
            default = "conf@1";
            conf@1 {
                description = "Boot Linux kernel with FDT";
                kernel = "kernel@1";
                fdt = "fdt@1";
                signature@1 {
                    algo = "rsa2048";
                    key-name-hint = "devkey";
                    sign-images = "kernel@1", "fdt@1";
                };
            };
        };
    };
   ```

3. Firmar la imagen:
   ```bash
   mkimage -f fit.its fit.itb
   ```

4. Configurar U-Boot:
   ```bash
   env set bootfile fit.itb
   env set verify=y
   bootm ${loadaddr}
   ```

**Ventaja:** Impide la ejecución de firmware no autorizado.

---

## 4. Encriptado de datos y archivos (`openssl`, `gpg`)

Proteger datos almacenados o transmitidos es esencial.

### `openssl`
Herramienta versátil para cifrado:

```bash
# Encriptar archivo
openssl enc -aes-256-cbc -salt -in secreto.txt -out secreto.enc
# Desencriptar
openssl enc -d -aes-256-cbc -in secreto.enc -out secreto.txt
```

### `gpg`
Permite cifrado asimétrico con gestión de claves públicas/privadas.

```bash
# Generar claves
gpg --gen-key

# Encriptar
gpg -e -r usuario@example.com archivo.txt

# Desencriptar
gpg -d archivo.txt.gpg
```

**Aplicaciones:** Backup de configuraciones, datos de sensores, logs.

---

## 5. Técnicas de encriptado de código y binarios

Para dificultar el análisis y la ingeniería inversa, se puede:

- **Ofuscar el código fuente** (cuando se distribuye): `strip`, `upx`.
- **Firmar binarios**: hash y firma digital para verificación.
- **Encriptar ejecutables**: menos común, pero usado en sectores críticos.

**Ejemplo con `strip`:**
```bash
strip mi_aplicacion
```

**Ejemplo con `upx`:**
```bash
upx --best binario
```

**Firma del binario con `openssl`:**
```bash
openssl dgst -sha256 -sign priv.key -out firma.bin binario
openssl dgst -sha256 -verify pub.key -signature firma.bin binario
```

---

## 6. Gestión de claves de seguridad en hardware embebido

Las **claves criptográficas** son la base de la autenticación y el cifrado. Su gestión segura es crítica.

**Buenas prácticas:**
- No almacenar claves en texto plano.
- Usar almacenamiento protegido: OTP, eFuses, TPM, Secure Elements.
- Cambiar claves por defecto antes del despliegue.

**Ejemplo de hardware:**
- **TPM (Trusted Platform Module)** con `tpm2-tools`:
  ```bash
  tpm2_getrandom --hex 8
  tpm2_createprimary -C o -g sha256 -G rsa -c primary.ctx
  ```
- **ATECC608A** (Microchip): almacena claves y realiza operaciones de firma/cifrado sin exponer la clave.

---

## 7. Estrategias para almacenamiento seguro de claves

Para evitar que claves se extraigan del dispositivo:

- **Key wrapping**: clave maestra protege otras claves.
- **Almacenamiento en particiones cifradas (dm-crypt, LUKS)**.
- **Hardware aislado (HSM o Secure Element)**.
- **Root of Trust**: clave raíz en OTP o ROM.

**Ejemplo:**
```bash
cryptsetup luksFormat /dev/mmcblk0p3
cryptsetup open /dev/mmcblk0p3 securedata
mkfs.ext4 /dev/mapper/securedata
mount /dev/mapper/securedata /mnt/data
```

**Nota:** Combinar técnicas según el nivel de seguridad requerido.

---

## 8. Uso de SELinux y AppArmor en entornos embebidos

**SELinux** y **AppArmor** son frameworks de control de acceso obligatorio (MAC).

### SELinux
- Usa políticas definidas por contexto (usuarios, procesos, objetos).
- Más complejo de configurar.
- Requiere soporte en el kernel y espacio de usuario.

**Ver políticas activas:**
```bash
sestatus
``` 

**Listar contextos:**
```bash
ls -Z /bin/ls
```

### AppArmor
- Más sencillo.
- Basado en rutas de archivos.
- Ideal para sistemas embebidos con menos complejidad.

**Ejemplo en Yocto:** añadir `meta-security` y activar AppArmor en `local.conf`:
```conf
DISTRO_FEATURES_append = " apparmor"
```

**Activar perfil:**
```bash
aa-enforce /etc/apparmor.d/myapp
```

Recursos:  
[meta-security](https://layers.openembedded.org/layerindex/branch/master/layer/meta-security/)  
[meta-selinux](https://layers.openembedded.org/layerindex/branch/master/layer/meta-selinux/)  
[App Armor cheatsheet](../assets/apparmor.md)

## 🛡️ Comparativa: SELinux vs AppArmor

| Característica                  | SELinux                                        | AppArmor                                      |
|--------------------------------|------------------------------------------------|-----------------------------------------------|
| **Modelo de seguridad**        | Basado en etiquetas (label-based)             | Basado en rutas de archivos (path-based)      |
| **Grano de control**           | Muy fino, control total sobre objetos         | Menos granular, más fácil de entender         |
| **Complejidad**                | Alta                                           | Moderada                                      |
| **Facilidad de configuración** | Más complejo (requiere política completa)     | Más sencillo (perfil por aplicación)          |
| **Flexibilidad**               | Alta (control sobre casi todos los aspectos)  | Limitada a archivos y procesos definidos      |
| **Soporte en distros**         | Fedora, RHEL, CentOS, Debian (opcional)       | Ubuntu, Debian (por defecto), SUSE            |
| **Herramientas disponibles**   | `semanage`, `audit2allow`, `setenforce`       | `aa-genprof`, `aa-enforce`, `aa-complain`     |
| **Tipo de políticas**          | Módulos con políticas complejas (MLS, RBAC)   | Perfiles más simples por aplicación           |
| **Soporte para contenedores**  | Integrado en RHEL + Podman/Kubernetes         | Soporte básico (mejor con LXD, Snap)          |
| **Documentación y comunidad**  | Amplia pero más técnica                       | Más amigable para principiantes               |
| **Interoperabilidad**          | Mejor en entornos con muchas etiquetas        | Más limitado en sistemas heterogéneos         |

> ⚠️ Ambos sistemas **no pueden funcionar simultáneamente** en modo enforcing.


## 🌿 Modelo Label-Based en SELinux

```text
+----------------------+        intenta acceder a         +----------------------+
|  Proceso: httpd_t    |  ----------------------------->  |  Archivo: index.html |
+----------------------+                                   +----------------------+
   Etiqueta SELinux:                                       Etiqueta SELinux:
   system_u:system_r:httpd_t:s0                            system_u:object_r:httpd_sys_content_t:s0

                            🔍 Evaluación de política
                            --------------------------------
                            ¿Puede un proceso con tipo httpd_t
                            acceder a un objeto con tipo
                            httpd_sys_content_t?

                                     ✅ SÍ (permitido por la política)
                                     ❌ NO (bloqueado por SELinux)
```

---

### 🧠 Claves del Modelo Label-Based

- **Proceso** (sujeto): tiene una etiqueta que indica su tipo (`httpd_t`).
- **Objeto** (archivo, directorio, etc.): tiene otra etiqueta (`httpd_sys_content_t`).
- La política de SELinux define qué interacciones entre tipos están **permitidas** o **denegadas**.

> Esto permite un modelo de seguridad **más robusto y flexible** que los permisos tradicionales.



---

## 9. Principios de seguridad en capas en Linux embebido

**Defensa en profundidad** implica aplicar múltiples capas de seguridad:

1. **Secure Boot**: verificar integridad desde el arranque.
2. **Cifrado de datos**: proteger datos en reposo.
3. **Usuarios limitados**: evitar ejecutar como root.
4. **Actualizaciones firmadas**: prevenir firmware malicioso.
5. **Firewall** y control de puertos:
   ```bash
   iptables -A INPUT -p tcp --dport 22 -j ACCEPT
   iptables -P INPUT DROP
   ```
6. **MAC (SELinux/AppArmor)**: restringir acceso a nivel de proceso.
7. **Logs de auditoría y detección de intrusiones (AIDE, fail2ban)**

**Ejemplo:** Un gateway IoT que usa U-Boot + Secure Boot, RootFS cifrado, políticas AppArmor y autenticación SSH con clave pública.

---

## 10. Buenas prácticas para el desarrollo seguro en Linux embebido

- Usar versiones actualizadas del kernel y librerías.
- Aplicar parches de seguridad regularmente.
- Validar entradas del usuario (evitar buffer overflows).
- Deshabilitar servicios innecesarios.
- Habilitar logs de auditoría (`rsyslog`, `logrotate`).
- Usar herramientas como:
  - `checksec` para ver protecciones de binarios
  - `scanelf` para detectar enlaces inseguros o funciones peligrosas
  - `lynis` para auditoría general del sistema
  - `clamav` para análisis de malware

**Ejemplo `checksec`:**
```bash
checksec --file=binario
```

**Ejemplo `scanelf`:**
```bash
scanelf -qR /usr/bin | grep 'libc.so'
```

**Ejemplo `lynis`:**
```bash
lynis audit system
```

**Simulación de ataques:**
- Usar fuzzers como `AFL` para encontrar vulnerabilidades.
- Pentesting con `nmap`, `metasploit`, etc. en entorno de pruebas controlado.

---

