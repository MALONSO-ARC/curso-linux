# Ejemplo IOCTLs (paso de datos entre kernel y userspace)

## Codigo del drivel de Linux

```C
/* ioctl_example.c - Driver de ejemplo con IOCTLs */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_dev"
#define CLASS_NAME "ioctl_class"

#define IOCTL_MAGIC 'x'
#define IOCTL_WRITE_DATA _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_READ_DATA _IOR(IOCTL_MAGIC, 2, int)

static int major;
static struct class* ioctl_class = NULL;
static struct device* ioctl_device = NULL;
static struct cdev ioctl_cdev;

static int stored_value = 0;

static int dev_open(struct inode *inodep, struct file *filep) {
    pr_info("IOCTL device opened\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    pr_info("IOCTL device closed\n");
    return 0;
}

static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    int user_data;
    switch (cmd) {
        case IOCTL_WRITE_DATA:
            if (copy_from_user(&user_data, (int __user *)arg, sizeof(int)))
                return -EFAULT;
            stored_value = user_data;
            pr_info("Valor recibido del espacio de usuario: %d\n", stored_value);
            break;
        case IOCTL_READ_DATA:
            user_data = stored_value;
            if (copy_to_user((int __user *)arg, &user_data, sizeof(int)))
                return -EFAULT;
            pr_info("Valor enviado al espacio de usuario: %d\n", user_data);
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .unlocked_ioctl = dev_ioctl,
    .release = dev_release,
};

static int __init ioctl_init(void) {
    dev_t dev;
    alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    major = MAJOR(dev);

    cdev_init(&ioctl_cdev, &fops);
    cdev_add(&ioctl_cdev, dev, 1);

    ioctl_class = class_create(THIS_MODULE, CLASS_NAME);
    ioctl_device = device_create(ioctl_class, NULL, dev, NULL, DEVICE_NAME);

    pr_info("IOCTL module cargado.\n");
    return 0;
}

static void __exit ioctl_exit(void) {
    device_destroy(ioctl_class, MKDEV(major, 0));
    class_unregister(ioctl_class);
    class_destroy(ioctl_class);
    cdev_del(&ioctl_cdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);
    pr_info("IOCTL module descargado.\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Curso Linux Embebido");
MODULE_DESCRIPTION("Driver ejemplo para IOCTL");
MODULE_VERSION("0.1");
```

## Código de espacio de usuario

```C
/* user_ioctl.c - Código de espacio de usuario */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_FILE "/dev/ioctl_dev"

#define IOCTL_MAGIC 'x'
#define IOCTL_WRITE_DATA _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_READ_DATA _IOR(IOCTL_MAGIC, 2, int)

int main() {
    int fd, value;

    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Error al abrir el dispositivo");
        return -1;
    }

    // Enviar valor al driver
    value = 1234;
    if (ioctl(fd, IOCTL_WRITE_DATA, &value) == -1) {
        perror("Error en IOCTL_WRITE_DATA");
        close(fd);
        return -1;
    }
    printf("Valor %d enviado al kernel\n", value);

    // Leer valor desde el driver
    value = 0;
    if (ioctl(fd, IOCTL_READ_DATA, &value) == -1) {
        perror("Error en IOCTL_READ_DATA");
        close(fd);
        return -1;
    }
    printf("Valor recibido del kernel: %d\n", value);

    close(fd);
    return 0;
}
```