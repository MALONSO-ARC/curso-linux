# Cifrado y descifrado de archivos de configuración

**Objetivo:** Proteger archivos sensibles usando cifrado simétrico y asimétrico.

## Pasos:
1. **Crear archivo de configuración:**
   ```bash
   echo "SSID=RedPrivada\nPASSWORD=supersecreto" > config.txt
   ```

2. **Cifrado con `openssl`:**
   ```bash
   openssl enc -aes-256-cbc -salt -in config.txt -out config.enc
   rm config.txt
   openssl enc -d -aes-256-cbc -in config.enc -out config.txt
   ```

3. **Cifrado con `gpg`:**
   ```bash
   gpg --gen-key
   gpg -e -r tu_correo@example.com config.txt
   rm config.txt
   gpg -d config.txt.gpg > config.txt
   ```