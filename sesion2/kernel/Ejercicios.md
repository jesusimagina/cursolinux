### 1 - Arrancamos el docker

	docker run -it --rm  -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux  --privileged jesusimagina/sdk-arm:1.0 /bin/bash


### 2 - Instalamos Dependencias

	apt install build-essential libncurses-dev bison flex libssl-dev libelf-dev bc

Hacemos un docker commit para guardar esta version. Desde el host:

	docker commit cursolinux sdk-arm:kernel
	
### 3 - Comprobamos nuestra versión del kernel

	uname -r

### 4 - Descargamos y descomprimimos el kernel 

En el host: Una versión posterior a la que tenemos en general el que descargamos el otro día

### 5 - Configuramos el kernel

	make menuconfig

Podemos modificar la versión LOCALVERSION

### 6 - Compilar

	make -j $(nproc)

### 7 - Podemos revisar las version

	make kernelrelease kernelversion image_name

### 8 - Instalar modulos

Desde el host 

	sudo make modules_install 
 
	sudo make install

El destino será /lib/modules para los módulos  y /boot para el config, el bzImage, el vmlinux y el System.map

### 9 - Actualizamos el grup

update-grub
update-grub2 
update-initramfs -u

### 10 - Borramos kernel
cd /boot
rm config-
rm initrd.img-
rm System.map-
rm vmlinuz-

### 11 - Borrar Módulos
cd /lib/modules
rm -rf 6.6.0

### 12 - Actualización de GRUB
update-grub
update-grub2

### 13 - Reiniciar
reboot



###### Configuramos correctamente

Limpiamos

	make clean
	make disclean

Compiamos configuracion anterior
	
	cp /boot/config- .config

Configuramos

	make oldconfig

Compilamos

	make -j$(nproc)

### Da problemas con el certificado

sudo mkdir -p /usr/local/src/debian
sudo apt install linux-source
sudo cp -v /usr/src/linux-source-*/debian/canonical-*.pem /usr/local/src/debian/
sudo apt purge linux-source*
...which:

installs the Ubuntu Linux kernel sources
copies the certificates into a dedicated non-package-managed directory
removes any kernel sources package again
