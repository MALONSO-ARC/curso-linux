## 🧪 Ejercicio 1: Análisis de hardware de almacenamiento de la BeagleBone Black

### Objetivo
Aprender a identificar los componentes de almacenamiento que incorpora la BeagleBone Black mediante el análisis de su documentación oficial.

### Recurso base
- https://docs.beagle.cc/boards/beaglebone/black/ch06.html#

### Instrucciones
1. Accede al enlace proporcionado.
2. Localiza la información sobre la memoria eMMC y el conector de tarjeta microSD.
3. Busca el modelo exacto del chip eMMC usado.
4. Investiga qué tipo de memoria es (raw NAND o no, si tiene controlador, interfaz, etc).
5. Verifica si la placa incluye alguna memoria NOR o SPI flash para el bootloader.
6. Consulta el esquema bloque del procesador AM335x para ver qué interfaz usa para acceder a la eMMC y la SD.

### Preguntas al alumno
- ✅ ¿Cuál es el modelo del chip de memoria eMMC incorporado?
- ✅ ¿Qué tipo de memoria es: raw NAND o con controlador?
- ✅ ¿Qué ventajas tiene este tipo de memoria para sistemas embebidos?
- ✅ ¿La BeagleBone Black puede arrancar desde la microSD? ¿Cuándo?
- ✅ ¿Se menciona algún tipo de memoria flash adicional (como SPI NOR)? 
- ✅ ¿Qué interfaz usa el procesador para comunicarse con la eMMC?


