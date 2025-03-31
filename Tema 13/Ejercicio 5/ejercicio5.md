# Activar AppArmor y crear perfil

**Objetivo:** Restringir acceso de una aplicación con AppArmor.

### Pasos:
1. **Verificar AppArmor:**
   ```bash
   sudo aa-status
   ```

2. **Crear script de prueba:**
   ```bash
   echo -e '#!/bin/bash\ncat /etc/passwd\ncat /home/usuario/archivo.txt' > prueba.sh
   chmod +x prueba.sh
   ```

3. **Generar perfil AppArmor:**
   ```bash
   sudo aa-genprof ./prueba.sh
   ```
   - Ejecuta el script cuando se te indique.
   - Deniega el acceso a `/home/usuario/archivo.txt`.

4. **Activar perfil:**
   ```bash
   sudo aa-enforce /etc/apparmor.d/usr.bin.prueba
   ./prueba.sh
   ```

5. **Ver logs:**
   ```bash
   dmesg | grep apparmor
   journalctl -xe | grep DENIED
   ```