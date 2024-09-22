# Integrar un módulo al kernel

### 1 - Creamos la receta para el módulo

Creamos la receta que se llame recipes-kernel en nuestro layer

	mkdir -p /work/layers/meta-hello/recipes-kernel/hellokm/files

### 2 - Creamos el hellokm.c

Editamos .c en /work/layers/meta-hello/recipes-kernel/hellokm/files/hellokm.c


	#include <linux/init.h>
	#include <linux/module.h>

	MODULE_AUTHOR("Profesor de SE");
	MODULE_DESCRIPTION("El primer modulo que saluda");
	MODULE_LICENSE("GPL");
	MODULE_VERSION("1.0");

	static int __init hellokm_init(void)
	{
		printk(KERN_INFO "Hello World from Kernel\n");
		return 0;
	}

	static void __exit hellokm_exit(void)
	{
		printk(KERN_INFO "Goodbye World\n");
	}

	module_init(hellokm_init);
	module_exit(hellokm_exit);
	
Editamos el /work/layers/meta-hello/recipes-kernel/hellokm/files/Makefile

	obj-m := hellokm.o

	SRC := $(shell pwd)

	all:
		$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

	modules_install:
		$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

	clean:
		rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
		rm -f Module.markers Module.symvers modules.order
		rm -rf .tmp_versions Modules.symvers

### 3 - Creamos el fichero de la receta

Editamos el /work/layers/meta-hello/recipes-kernel/hellokm/hellokm.bb

	SUMMARY = "My Module"
	LICENSE = "GPLv2"
	LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

	inherit module

	SRC_URI = "file://Makefile \
		   file://hellokm.c \
		  "

	S = "${WORKDIR}"

	# The inherit of module.bbclass will automatically name module packages with
	# "kernel-module-" prefix as required by the oe-core build environment.

	RPROVIDES_${PN} += "kernel-module-hellokm"
	
### 4 - Podemos compilarlo

	bitbake hellokm
	
### 5 - Generamos la imagen de nuevo

Añadimos al local.conf

	MACHINE_ESSENTIAL_EXTRA_RDEPENDS += "kernel-module-hellokm"
	KERNEL_MODULE_AUTOLOAD += "hellokm"

y generemos
	
	bitbake core-image-minimal
	
Probamos con runquemu

	runqemu qemuarm nographic
	
