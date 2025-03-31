## 🧾 Cheat Sheet: `libgpiod` – Control de GPIOs en Linux moderno

---

### 📦 Instalación
```bash
sudo apt install gpiod libgpiod-dev
```

---

### 🔧 Herramientas CLI

| Comando              | Descripción                                      |
|----------------------|--------------------------------------------------|
| `gpiodetect`         | Lista los chips GPIO disponibles (`/dev/gpiochip*`) |
| `gpioinfo gpiochip0` | Muestra info de las líneas GPIO del chip         |
| `gpioget gpiochip0 N`| Lee el estado del GPIO `N`                        |
| `gpioset gpiochip0 N=1` | Pone en alto (`1`) el GPIO `N`               |
| `gpiomon gpiochip0 N`| Espera eventos (cambios) en la línea `N`         |

#### 📌 Ejemplo de uso:
```bash
gpiodetect
gpioinfo gpiochip0
gpioset gpiochip0 23=1
gpioget gpiochip0 23
gpiomon gpiochip0 24  # Espera rising/falling edge en GPIO 24
```

---

### 💻 Uso en C: funciones clave

#### 🔹 Inicializar y obtener línea
```c
struct gpiod_chip *chip = gpiod_chip_open_by_name("gpiochip0");
struct gpiod_line *line = gpiod_chip_get_line(chip, 24);
```

#### 🔹 Solicitar línea como salida
```c
gpiod_line_request_output(line, "myapp", 0);  // valor inicial 0
gpiod_line_set_value(line, 1);
```

#### 🔹 Solicitar línea como entrada
```c
gpiod_line_request_input(line, "myapp");
int value = gpiod_line_get_value(line);
```

---

### ⚡ Detección de eventos GPIO (interrupciones)

#### 🛠️ CLI (gpiomon)
```bash
gpiomon gpiochip0 24
```

#### 💻 C: esperar evento (edge detection)
```c
#include <gpiod.h>
#include <stdio.h>

int main() {
    struct gpiod_chip *chip = gpiod_chip_open_by_name("gpiochip0");
    struct gpiod_line *line = gpiod_chip_get_line(chip, 24);

    // Solicita la línea para eventos
    gpiod_line_request_both_edges_events(line, "event-listener");

    printf("Esperando eventos en GPIO 24...\n");

    while (1) {
        struct gpiod_line_event event;
        int ret = gpiod_line_event_wait(line, NULL);  // espera bloqueante
        if (ret == 1 && gpiod_line_event_read(line, &event) == 0) {
            if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE)
                printf("Rising edge!\n");
            else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
                printf("Falling edge!\n");
        }
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
```

---

### 🧠 Tip extra: usar `poll()` para timeout no bloqueante
```c
#include <poll.h>
struct pollfd pfd = {
    .fd = gpiod_line_event_get_fd(line),
    .events = POLLIN,
};
int ret = poll(&pfd, 1, 1000);  // 1000 ms timeout
```

---

### 📚 Enlaces útiles
- Docs oficiales: [libgpiod docs](https://libgpiod.readthedocs.io/)  
- Código fuente y ejemplos: [libgpiod code](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/)

