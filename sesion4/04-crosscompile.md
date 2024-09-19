### 1 - Arrancamos el docker

En el directorio de trabajoy tenemos que tener en /work del docker:

	# ls
	# kernelsrc drivers rootfs
	# ls rootfs
	# bin  dev  etc  home  lib  linuxrc  proc  sbin  sys  tmp  usr  var


	docker run -it --rm  -v $(pwd):/work -w /work -v /lib/modules:/lib/modules --hostname cursolinux --name cursolinux  --privileged jesusimagina/sdk-arm:latest /bin/bash

e instalamos 

	apt install kmod

### 2 - Esportamos variables

	export ARCH=arm
	export CROSS_COMPILE=/opt/sdk-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
	
### 3 - Compilamos el kernel e instalamos los módulos

	make zImage
	make modules
	make dtbs
	make -C compile/linux/ INSTALL_MOD_PATH=/work/rootfs/ modules_install	
	
### 4 - Creamos una carpeta para los drivers

Compilamos el driver modificando el BUILDDIR del Makefile


### 5 -Probamos con qemu

Copiamos todos los binarios a una carpeta qemu

### 6 - Integramos en el código del kernel

### 7 - Creamos una entrada en el menu 

En menuconfig del kernel debe tener una entrada para configurarlo como modulo o como parte del zImage
