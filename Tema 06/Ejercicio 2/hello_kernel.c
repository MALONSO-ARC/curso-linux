/* hello_world.c - Un driver simple de "Hello World" para el kernel de Linux */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Un driver simple de Hello World");
MODULE_VERSION("1.0");

/* Función que se ejecuta cuando el módulo es cargado */
static int __init hello_init(void) {
    printk(KERN_INFO "[HelloWorld] Módulo cargado: ¡Hola, mundo desde el kernel!\n");
    return 0;
}

/* Función que se ejecuta cuando el módulo es removido */
static void __exit hello_exit(void) {
    printk(KERN_INFO "[HelloWorld] Módulo descargado: Adiós, mundo.\n");
}

module_init(hello_init);
module_exit(hello_exit);