### Dependencias de recetas

### 1 - Creamos una nueva receta

Creamos una receta con la herramienta devtool y con un repositorio de terceros

	devtool add https://github.com/yksz/c-logger.git

Genera los archivos en el workspace

### 2 - Editar la receta

Podemos dar un primer vistazo a la receta

	devtool edit-recipe logger

Al haberle puesto un repositorio de git, no necesitamos implementar do_fetch porque yocto ya sabe lo que tiene que hacer
Implementamos el resto de funciones sobre todo do_compile y do_install 

Añadimos al fichero;

	do_configure(){
	    ${S}/build.sh
	}

	do_compile(){
	    make -C ${S}/build
	}

	do_install(){
	    make -C ${S}/build install DESTDIR=${D}
	    cd ${D}/usr/local/lib
	    mv liblogger.so liblogger.so.1.0 
	    ln -s liblogger.so.1.0 liblogger.so
	    chmod 755 liblogger.so.1.0
	    cp -R ${D}/usr/local/* ${D}/usr
	    rm -rf ${D}/usr/local/
	}

	FILES_${PN} += "/usr/lib/*"
	FILES_${PN} += "/usr/include/*"
	
### Construimos el paquete

devtool también puede compilar el paquete de modo similar a bitbake con el subcomando build

	devtool build logger


### Instalamos el paquete

Nuestra distribución no tiene el paquete ssh instalado por lo que se lo incluimos. Vamos a hacerlo añadiendo en el conf/local el paquete en EXTRA_IMAGE_FEATURES

	EXTRA_IMAGE_FEATURES ?= "debug-tweaks ssh-server-openssh"
	
Generamos la imagen de nuevo

	devtool build-image core-image-minimal
	
Arrancamos al imagen con qemuarm

	runqemu qemuarm nographic

Arrancar desde otra terminal del host otra sesión de docker

	docker exec -it cursolinux /bin/bash

Dentro de la sesión ejecutar el source

	source poky/oe-init-build-env build-qemuarm

Esto nos permitirá instalar nuestro paquete en el destino
	
	devtool deploy-target logger root@192.168.7.2

Podemos comprobar en el destino que se encuentra en /usr/lib

Si quisieramos quitarlo

	devtool undeploy-target logger

### Para añadir la receta a nuestra capa

	devtool finish -f logger ../layers/meta-hello
	rm -rf workspace/sources/logger	
