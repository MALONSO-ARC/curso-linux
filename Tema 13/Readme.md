# Tema 13: Seguridad en Linux Embebido

La seguridad en sistemas embebidos es fundamental, especialmente en sectores como el IoT, automoción o industrial, donde una vulnerabilidad puede comprometer dispositivos en campo, privacidad de usuarios o incluso la seguridad física.

---

## 1. Conceptos básicos de seguridad en sistemas embebidos

Los sistemas embebidos presentan desafíos de seguridad particulares debido a sus restricciones de hardware, su ciclo de vida largo y su despliegue en entornos potencialmente hostiles.

**Principales características de seguridad:**
- Confidencialidad: proteger los datos del usuario.
- Integridad: asegurar que los datos y firmware no han sido modificados.
- Autenticidad: verificar la identidad de firmware, usuarios o servicios.
- Disponibilidad: garantizar que el sistema está accesible y operativo.

**Ejemplo:** Una cámara IP conectada a internet debe proteger las credenciales de acceso, evitar que su firmware sea modificado por terceros, y mantenerse operativa incluso ante ataques.

---

## 2. Modelado de amenazas y análisis de riesgos

Antes de implementar contramedidas, es vital identificar posibles vectores de ataque y evaluar el impacto que tendrían.

**Pasos comunes:**
1. Identificar activos (firmware, credenciales, interfaces de red...)
2. Evaluar amenazas (ataques físicos, MITM, actualización maliciosa...)
3. Analizar vulnerabilidades (puertos abiertos, protocolos inseguros...)
4. Estimar el riesgo: _riesgo = impacto × probabilidad_

**Herramientas útiles:** STRIDE, DREAD, TARA (Threat Analysis and Risk Assessment).

**Ejemplo:** En un gateway IoT se evalúa la posibilidad de un ataque remoto que reemplace el firmware. Se valora implementar autenticación de actualizaciones y arranque seguro.

---

## 3. Introducción a Secure Boot y su implementación en U-Boot

**Secure Boot** garantiza que el firmware ejecutado en el arranque es legítimo y no ha sido modificado.

### ¿Cómo funciona?
- Se almacena en el hardware una clave pública o hash verificado.
- El bootloader verifica la firma digital del siguiente componente (kernel, RootFS).
- Si la firma es válida, el sistema continúa. Si no, se detiene el arranque.

### U-Boot y Secure Boot
U-Boot admite firma de imágenes usando **FIT (Flattened Image Tree)** con soporte de verificación RSA.

**Pasos típicos:**
1. Crear claves RSA (openssl).
2. Firmar el FIT con el kernel, DTB y ramdisk.
3. Configurar U-Boot para validar firmas.

```bash
mkimage -f fit.its fit.itb
openssl genrsa -out key.pem 2048
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

**Nota:** Esto no reemplaza Secure Boot, pero es una capa adicional.

---

## 6. Gestión de claves de seguridad en hardware embebido

Las **claves criptográficas** son la base de la autenticación y el cifrado. Su gestión segura es crítica.

**Buenas prácticas:**
- No almacenar claves en texto plano.
- Usar almacenamiento protegido: OTP, eFuses, TPM, Secure Elements.
- Cambiar claves por defecto antes del despliegue.

**Ejemplo de hardware:**
- **TPM (Trusted Platform Module)**
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
```

**Nota:** Combinar técnicas según el nivel de seguridad requerido.

---

## 8. Uso de SELinux y AppArmor en entornos embebidos

**SELinux** y **AppArmor** son frameworks de control de acceso obligatorio (MAC).

### SELinux
- Usa políticas definidas por contexto (usuarios, procesos, objetos).
- Más complejo de configurar.
- Requiere soporte en el kernel y espacio de usuario.

### AppArmor
- Más sencillo.
- Basado en rutas de archivos.
- Ideal para sistemas embebidos con menos complejidad.

**Ejemplo en Yocto:** añadir `meta-security` y activar AppArmor en `local.conf`:
```conf
DISTRO_FEATURES_append = " apparmor"
```

---

## 9. Principios de seguridad en capas en Linux embebido

**Defensa en profundidad** implica aplicar múltiples capas de seguridad:

1. **Secure Boot**: verificar integridad desde el arranque.
2. **Cifrado de datos**: proteger datos en reposo.
3. **Usuarios limitados**: evitar ejecutar como root.
4. **Actualizaciones firmadas**: prevenir firmware malicioso.
5. **Firewall** y control de puertos.
6. **MAC (SELinux/AppArmor)**: restringir acceso a nivel de proceso.

**Ejemplo:** Un gateway IoT que usa U-Boot + Secure Boot, RootFS cifrado, políticas AppArmor y autenticación SSH con clave pública.

---

## 10. Buenas prácticas para el desarrollo seguro en Linux embebido

- Usar versiones actualizadas del kernel y librerías.
- Aplicar parches de seguridad regularmente.
- Validar entradas del usuario (evitar buffer overflows).
- Deshabilitar servicios innecesarios.
- Habilitar logs de auditoría.
- Usar herramientas como `checksec`, `scanelf`, `lynis`, `clamav`.
- Simular ataques (pentesting, fuzzing) en entornos de prueba.


