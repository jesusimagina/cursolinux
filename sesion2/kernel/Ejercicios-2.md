### Dentro del docker 

	docker run -it --rm  -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux  --privileged jesusimagina/sdk-arm:sdk-arm:kernel /bin/bash

### Exportamos el path de la toolchain

	export TOOLCHAIN=/opt/sdk-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
	export LINUXSRC=/work/linux-6.10.7

# Compilamos la imagen

Compilaremos una imagen para QEMU


Dentro de la carpeta de los fuentes:

	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C $LINUXSRC  mrproper
	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C $LINUXSRC versatile_defconfig O=/work/compile/linux-qemu
	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C /work/compile/linux-qemu zImage
	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C /work/compile/linux-qemu modules
	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C /work/compile/linux-qemu dtbs

### Probamos con qemu

	QEMU_AUDIO_DRV=none qemu-system-arm -m 256M -nographic -M versatilepb -kernel ./arch/arm/boot/zImage -append "console=ttyAMA0,115200" -dtb ./arch/arm/boot/dts/arm/versatile-pb.dtb

# Configuramos 

	make ARCH=arm -C /work/compile/linux-qemu menuconfig

	CONFIG_PRINTK_TIME=y

# Volvemos a compilar

