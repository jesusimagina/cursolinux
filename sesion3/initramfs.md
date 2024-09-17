### 1 - Arrancamos al imagen de docker

Lo ideal es tenerlo en un directorio donde tengamos la compilación del kernel

	docker run -it --rm  -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux  --privileged jesusimagina/sdk-arm:latest /bin/bash


### 2 - Descargamos el busybox 

Dentro de la carpeta /work/rootfs

	wget https://busybox.net/downloads/busybox-1.36.1.tar.bz2
	tar -xjf busybox-1.36.1.tar.bz2
	cd busybox-1.36.1

### 3 - Configuramos el busybox

Seleccionar "Build BusyBox as a static binary (no shared libs)"
Navega a "Networking Utilities" y desactiva "tc"

### 4 - Compilamos

	export ARCH=arm
	export CROSS_COMPILE=/opt/sdk-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
	make -C busybox-1.36.1 menuconfig O=/work/rootfs/compile/busybox
	make -j$(nproc) -C /work/rootfs/compile/busybox
	make install -C /work/rootfs/compile/busybox

### 5 - Creamos el sistema de archivos

En el directorio de compilación creamos el rootfs que será nuestra imagen:
	
	mkdir /work/rootfs/compile/rootfs/
	cd /work/rootfs/compile/rootfs
	mkdir bin  dev  etc  home  lib  proc  sbin  sys  tmp  usr  var
	mkdir usr/bin usr/lib usr/sbin
	mkdir var/log

Copiamos lo generado por el busybox

	cp -R /work/rootfs/compile/busybox/_install/* .

### 6 Instalamos las librerias de glibc y gcc

	export SYSROOT=$(${CROSS_COMPILE}gcc -print-sysroot)
	cp -a $SYSROOT/lib/ld-linux-armhf.so.3 lib
	cp -a $SYSROOT/lib/libc.so.6 lib
	cp -a $SYSROOT/lib/libm.so.6 lib
	cp -a $SYSROOT/../lib/lib*.so lib
	cp -a $SYSROOT/../lib/lib*.so* lib

Podemos hacer el strip si queremos que ocupe menos

	${CROSS_COMPILE}strip lib/lib*.so.*.*0

### 8 - Creamos dispositivos básicos

	mknod -m 666 dev/null c 1 3
	mknod -m 600 dev/console c 5 1

### 9 - Generamos la imagen 

Para es hemos de instalar el comando cpio

	apt install cpio 

	find . | cpio -o -H newc > ../initramfs.cpio

### 10 - Comprimimos la imagen

	cd ..
	gzip initramfs.cpio

### 11 - Simulamos con qemu

Creamos en /work una carpeta qemu

	mkdir /work/qemu

Copiamos todos los archivos que nos interesan:
 - zImage
 - versatile-pb.dtb
 - initramfs.cpio.gz

 
