# Instalar dependencias

	apt-get install qemu-user-static debootstrap binfmt-support
	
# Ejecutamos el bootstrap


	debootstrap --arch=arm64 --variant=minbase bookworm  /mnt/debian-min-arm http://ftp.debian.org/debian
	
o otra alternativa

	debootstrap --arch=arm64 --foreign bookworm  /mnt/debian-min-arm http://ftp.debian.org/debian
	chroot /mnt/debian-min-arm /bin/bash
	/debootstrap/debootstrap --second-stage
	
# Actualizamos el sistema con los paquetes que queramos
