# Modificar el kernel en yocto

Modificamos el Makefile del hellokm para que compile solo si esta habilitado en el kernel

	obj-$(CONFIG_HELLOKM) := hellokm.o
	
Vamos a la carpeta /work/build-qemuarm/tmp/work-shared/qemuarm/kernel-source donde esta el código del kernel
Editamos el fichero drivers/char/Kconfig

El principio del fichero quedará asi

	menu "Character devices"

	source "drivers/tty/Kconfig"

	config HELLOKM
		tristate "/dev/hellokm device support"
		default n
		help
		  Say Y here if you want to support the /dev/hellokm device.
		  The /dev/mem device is used to test kernel modules
		  When in doubt, say "m".
		  
Generamos la imagen

	bitbake core-image-minimal
	
Hacemos el runqemu y vemos que efectivamente hellokm no está

### Modificamos la configuración

Instalamos el paquete screen

	sudo apt install screen
	
Hacemos un menuconfig	
	
	bitbake -c menuconfig virtual/kernel
	
No modificamos nada pero observamos que el HELLOKM está

### Hacemos un parche con la modificación

Nos situamos en /work/build-qemuarm/tmp/work-shared/qemuarm/kernel-source

	git diff > mychanges.patch
	
### Creamos la receta

En nuestra capa de meta-drivers en recipes-kernel creamos la receta linux

	recipes-kernel/linux/

Editamos el linux-yocto_%.bbappend

	FILESEXTRAPATHS:prepend := "${THISDIR}/:"
	SRC_URI += "file://mychanges.patch"
	
Copiamos bajo recipes-kernel/linux/ el fichero *mychanges.patch*

Reconstruimos el kernel

	bitbake virtual/kernel
	
### Modificamos la configuración

Editamos bajo recipes-kernel/linux/hellokm-m.cfg 

	CONFIG_HELLOKM=m
