# Creamos un capa

### 1 - Creacrión de una capa

Generamos el entorno

	source poky/oe-init-build-env build-qemuarm

Creamos la capa con el comando bitbake-layers

	mkdir /work/layers
	bitbake-layers create-layer /work/layers/meta-hello

El árbol debe quedar:

	/work/
	|-- build-qemuarm
	|-- layers
	`-- poky

Y dentro de layers

	/work/layers/
	`-- meta-hello
	    |-- COPYING.MIT
	    |-- README
	    |-- conf
	    `-- recipes-example

### 2 - Añadimos la capa

Añadimos la capa al directorio de compilacióm

Desde el mismo directorio build-qemuarm

	bitbake-layers add-layer /work/layers/meta-hello


Podemos comprobar que se ha añadido

	bitbake-layers show-layers

O examinando el archivo *conf/bblayers.conf*

	# POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf
	# changes incompatibly
	POKY_BBLAYERS_CONF_VERSION = "2"

	BBPATH = "${TOPDIR}"
	BBFILES ?= ""

	BBLAYERS ?= " \
	  /work/poky/meta \
	  /work/poky/meta-poky \
	  /work/poky/meta-yocto-bsp \
	  /work/layers/meta-hello \
 	 "
### 3 - Añadimos una receta

Vamos a añadir una receta sencilla. El árbol de directorios quedará así:

	recipes-helloworld/helloworld/
	|-- files
	|   `-- helloworld.c
	`-- helloworld_1.0.bb

Dentro del directorio */work/layers/meta-hello* creamos una carpeta para la receta:

	cd /work/layers/meta-hello
	mkdir -p recipes-helloworld/helloworld

Dentro de la receta 

	cd recipes-helloworld/helloworld

Editamos un fichero llamado helloworld_1.0.bb con el siguiente contenido

	DESCRIPTION = "Un programa que dice Hello World!"
	PRIORITY = "optional"
	SECTION = "examples"
	LICENSE = "GPLv2"
	LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"
	SRC_URI = "file://helloworld.c"
	S = "${WORKDIR}"
	
	do_compile() {
	    ${CC} ${CFLAGS} ${LDFLAGS} helloworld.c -o helloworld
	}

	do_install() {
	    install -d ${D}${bindir}
	    install -m 0755 helloworld ${D}${bindir}
	}
	
Creamos el directorio files (Revise el árbol de directorios para hacerlo correctamente)

	mkdir files

Y editamos un programa en *c* llamado helloworld.c

	#include <stdio.h>

	int main()
	{
        	printf("Hello World\n");
	        return 0;
	}


### 4 - Horneamos de nuevo

Ejecutamos el bitbake pero para que haga la receta helloworld

Nos vamos al directorio de compilación

	cd /work/build-qemuarm

Y compilamos

	bitbake helloworld

Deberíamos tener un directorio en el *tmp/work*

	ls tmp/work/armv7vet2hf-neon-poky-linux-gnueabi/helloworld

Y el rpm en deploy

	tmp/deploy/rpm/armv7vet2hf_neon/helloworld-1.0-r0.armv7vet2hf_neon.rpm
	tmp/deploy/rpm/armv7vet2hf_neon/helloworld-dbg-1.0-r0.armv7vet2hf_neon.rpm
	tmp/deploy/rpm/armv7vet2hf_neon/helloworld-dev-1.0-r0.armv7vet2hf_neon.rpm
	tmp/deploy/rpm/armv7vet2hf_neon/helloworld-src-1.0-r0.armv7vet2hf_neon.rpm


Sin embargo, todavía no forma parte de la imagen de destino. La lista de paquetes que se instalarán se guarda en una variable denominada IMAGE_INSTALL.

### 5 - Agregarlo al rootfs de la imagen

Añadimos al conf/local.conf

	IMAGE_INSTALL_append = " helloworld"

Generamos la imagen de nuevo

	bitbake core-image-minimal

### 5 - Probamos 

	runqemu qemuarm nographic


