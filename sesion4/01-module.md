
# Escribir un módulo 

### Wordir

Creamos un directorio de trabajo para los drivers

	mkdir driver
	mkdir driver/first_module


Editamos un hellokm.c

Contendrá:

### Los includes que nos proporcionan el LKM

	#include <linux/init.h>
	#include <linux/module.h>

### Las macros obligatorias

 - MODULE_AUTHOR(): específica los autores del módulo del kernel
 - MODULE_DESCRIPTION(): Describe brevemente la función o propósito de este
 - MODULE_LICENSE(): Especifica las licencias bajo las cuales se lanza este módulo del kernel.
 - MODULE_VERSION(): especifica la cadena de versión (local) del módulo del kernel

	MODULE_AUTHOR("SEnsei");
	MODULE_DESCRIPTION("El primer modulo que saluda");
	MODULE_LICENSE("GPL");
	MODULE_VERSION("1.0");

### Funciones de entrada y salida

La implementación de la función E/S

	static int __init hellokm_init(void)
	{
        	printk(KERN_INFO "Hello World from Kernel\n");
	        return 0;
	}

	static void __exit hellokm_exit(void)
	{
        	printk(KERN_INFO "Goodbye World\n");
	}

Y sus macros:

	module_init(hellokm_init);
	module_exit(hellokm_exit);

### Makefile

	VLINUX = $(shell /bin/uname -r)
	BUILDDIR = /lib/modules/$(VLINUX)/build

	MODULE=hellokm
	PWD=$(shell pwd)
	obj-m := $(MODULE).o

	all:
                make -C $(BUILDDIR) M=$(PWD) modules
	clean:
		make -C $(BUILDDIR) M=$(PWD) modules

### Probamos

En un terminal:

	sudo dmesg -w

En el directorio de trabajo

	sudo insmod hellokm.ko

Podemos inspeccionarlo con las órdenes:

	 modinfo hellokm.ko

Ver si esta insertado:

	lsmod | grep hellokm

Para quitarlo 

	sudo rmmod hellokm
