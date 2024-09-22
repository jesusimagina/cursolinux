# Configurar el servidor de paquetes

### 1 - Editamos el conf/local.conf

	source poky/oe-init-build-env build-qemuarm

Editamos el conf/local.conf:

 - Comentamos que los paquetes sean rpm

	#PACKAGE_CLASSES ?= "package_rpm"
	PACKAGE_CLASSES ?= "package_ipk"

 - Añadimos algunas cosas a la distro

	EXTRA_IMAGE_FEATURES ?= "debug-tweaks ssh-server-openssh package-management"
	
 - Eliminamos las lineas de nuestros paquetes
 
	#IMAGE_INSTALL_append = " helloworld"
	#IMAGE_INSTALL_append = " logger"

### 2 - Generamos la imagen de nuevo 

	bitbake core-image-minimal
	
Después como hemos quitado los paquetes propios los rehacemos

	bitbake helloworld
	
Y actualizamos las dependencias

	bitbake package-index

### 3 - Despues de muuucho tiempo

Arrancar el emulador

	runqemu qemuarm
	
Comprobamos que tenemos el comando opkg instalado 

	which opkg	

### 4 - Creamos el servidor de paquetes

Vamos a donde están lso paquetes
	
	cd tmp/deploy/ipk/
	
Arrancamos el servidor con la ip de nuestro docker/host (la tap0)

	sudo python3 -m http.server --bind 192.168.7.1


### 5 - Vamos al emulador

Editamos el fichero opkg.conf

	vi /etc/opkg/opkg.conf
	
Y escribimos las siguientes líneas (Cambiando la ip para la de vuestro servidor):

	src/gz all http://192.168.7.1:8000/all
	src/gz armv7vet2hf-neon http://192.168.7.1:8000/armv7vet2hf-neon
	src/gz qemuarm http://192.168.7.1:8000/qemuarm
	
Actualizamod el opkg

	opkg update
	
### 6 - Instalamos paquetes

	opkg install helloworld
	
Deberían instalarse logger y helloworld

