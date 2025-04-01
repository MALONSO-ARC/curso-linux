## 🛡️ Linux en Sistemas Safety-Critical – Cheatsheet SIL2/SIL3

### 🔧 1. **ELISA Project** (Enabling Linux in Safety Applications)
- **Organiza**: Linux Foundation.
- **Objetivo**: Hacer de Linux un sistema usable en entornos safety-critical.
- **Enfoque**: Crear artefactos que ayuden a la certificación de sistemas que usan Linux.
- **Casos de uso**:
  - Industrial (IEC 61508)
  - Automoción (ISO 26262)
  - Médico (IEC 62304)
  - Aviación (DO-178C)
- **Sitio**: [https://elisa.tech](https://elisa.tech)

### 📆 2. **PREEMPT-RT** (Real-Time Linux)
- **Objetivo**: Proveer capacidades determinísticas a Linux.
- **Estado**: Muchas partes ya están upstream en el kernel principal.
- **Importancia**: Esencial para aplicaciones tiempo-real certificables.
- **Relación con Safety**: Es la base para distribuciones safety que necesitan determinismo.

### 🚗 3. **Automotive Grade Linux (AGL)**
- **Organiza**: Linux Foundation.
- **Enfoque**: Sistema operativo para el coche conectado.
- **Relación con Safety**: Colaboraciones con ISO 26262 y exploración de safety islands (Linux + MCU seguro).
- **Sitio**: [https://www.automotivelinux.org](https://www.automotivelinux.org)

### ✅ 4. **Open Source Automation Development Lab (OSADL)**
- **Rol**: Soporte a la industria para el uso de software libre en sistemas industriales.
- **Proyectos relevantes**:
  - Certificación de herramientas y componentes Linux (real-time kernel, toolchains).
  - Mantenimiento de una base de datos de determinismo del kernel.
- **Sitio**: [https://www.osadl.org](https://www.osadl.org)

### 🏎️ 5. **Safety Critical Linux (Codethink / ELISA)**
- **Meta**: Construcción de sistemas Linux safety-compliant usando herramientas open source como:
  - **Buildroot** / **Yocto**
  - **Systemd**, **GNU/Linux tools**
- **Certificación orientada**: Hacia IEC 61508 SIL2/SIL3.

### 📜 6. **CertLinux Initiative (Bosch, Siemens, etc.)**
- **Objetivo**: Certificación pre-aprobada de ciertas configuraciones Linux (ej: BOSCH en ISO 26262 ASIL B).
- **Estado**: Internamente usados, artefactos no siempre públicos.

### 🧱 7. **Safety Island Architecture**
- **Modelo**: Arquitectura mixta SoC: Linux en procesador principal + RTOS certificado en coprocesador/MCU.
- **Casos de uso**:
  - Linux para HMI, logging, comunicación.
  - RTOS para lógica safety (ej. FreeRTOS certificado, AUTOSAR).
- **Ventaja**: Permite aislar funcionalidades safety en entorno certificable.

---

## 📘 Normas Relacionadas

| Norma        | Sector         | Aplica a Linux |
|--------------|----------------|----------------|
| **IEC 61508**| Industrial     | ✅              |
| **ISO 26262**| Automoción     | ✅              |
| **IEC 62304**| Dispositivos médicos | ✅       |
| **DO-178C**  | Aeronáutica     | Parcial (non-safety) |

