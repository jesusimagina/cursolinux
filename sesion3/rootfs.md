# Instalamos el gcc para arm 64 bits 

	apt install gcc-aarch64-linux-gnu

# Compilamos el kernel de variscite

Compilamos el kernel de la placa var-som-mx8mn para la distribución debian:

https://variwiki.com/index.php?title=VAR-SOM-MX8M-MINI


# Compilar el busybox pero para arm64

	export ARCH=arm64 
	export CROSS_COMPILE=aarch64-linux-gnu-

Las librerías de la toolchain estan en: /usr/aarch64-linux-gnu/lib/

# Para generar una imagen de rootfs 

Verificamos cuanto ocupa nuestro sistema con du -h

Creamos la imagen

	cd /work/rootfs/compile/
	dd if=/dev/zero of=rootfs.img bs=1M count=32
	mkfs.ext4 rootfs.img
	mkdir mnt
	mount -o loop rootfs.img mnt/
	cp -R rootfs/* mnt/
	umount mnt

# Probar con qemu

	qemu-system-aarch64 -M virt -cpu cortex-a53 -m 1024 -nographic    -kernel Image.gz -append "console=ttyAMA0 root=/dev/vda init=/sbin/init"    -drive if=none,file=rootfs.img,format=raw,id=hd0    -device virtio-blk-device,drive=hd0

