# Implementación de Secure Boot con U-Boot y FIT

**Objetivo:** Firmar una imagen FIT y configurar U-Boot para verificarla antes de arrancar.

## Pasos:
1. **Generar claves RSA:**
   ```bash
   openssl genpkey -algorithm RSA -out devkey.key -pkeyopt rsa_keygen_bits:2048
   openssl rsa -in devkey.key -pubout -out devkey.pub
   ```

2. **Crear archivo `fit.its`:**
   - Guarda el siguiente contenido como `fit.its` (ajusta nombres si es necesario):
   ```dts
   /dts-v1/;

   / {
       description = "FIT image with kernel and FDT";
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
               description = "Device Tree";
               data = /incbin/("am335x-bonegreen.dtb");
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

3. **Firmar la imagen FIT:**
   ```bash
   mkimage -f fit.its fit.itb
   ```

4. **Configurar U-Boot:**
   ```bash
   env set bootfile fit.itb
   env set verify=y
   load mmc 0:1 ${loadaddr} fit.itb
   bootm ${loadaddr}
   ```

5. **Probar el sistema:**
   - Modifica el archivo `fit.itb` y verifica que U-Boot lo rechaza.

