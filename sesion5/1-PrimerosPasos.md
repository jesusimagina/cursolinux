# Ejercicio 1 - bitbake

### 1 - Dorcker run 

	docker run -it --rm  -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux  --privileged -u user ubuntu-poky:latest /bin/bash

Este docker tendrá un usuario con nuestros permisos.

### 2 -Descargamos Yocto

Dentro de la carpeta /work o su correspondiente carpeta compartida en nuestro host

	git clone -j$(nproc) -b dunfell git://git.yoctoproject.org/poky.git

### Generamos el directorio de trabajo

Borramos el fichero /work/build-qemuarm/conf/local.conf.

	rm /work/build-qemuarm/conf/local.conf

Y generamos el entorno

	source poky/oe-init-build-env build-qemuarm

Nos habrá situado en la carpeta build-qemuarm.

Editamos el fichero conf/local.conf y descomentamos la línea que pone:

	MACHINE ?= "qemuarm"

Comentamos la que pone:

	#MACHINE ??= "qemux86-64"

y añadimos las siguientes líneas:

	BB_NUMBER_THREADS?="5"
	PARALLEL_MAKE?="-j5"
 
El número 5 es dependiente de vuestro sistema. Cuantos más se puedan asignar mejor. 

Esto significa:

 - BB_NUMBER_THREADS: Número de tareas que se ejecutan en paralelo 
 - PARALLEL_MAKE: Número de paralelización de los Make
 - INHERIT: Indicamos que se borre el directorio de trabajo para ahorrar espacio


### Compilamos

	bitbake core-image-minimal

### Probamos

	runqemu qemuarm nographic


