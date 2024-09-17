# 1 - Arrancamos el docker

	docker run -it --rm  -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux  --privileged jesusimagina/sdk-arm:1.0 /bin/bash


docker run: Comando para ejecutar un contenedor.
-it: Modo interactivo con terminal.
--rm: Elimina el contenedor al salir.
-v $(pwd):/work: Monta el directorio actual ($(pwd)) en /work dentro del contenedor.
-w /work: Establece /work como el directorio de trabajo.
--hostname cursolinux: Asigna el nombre de host cursolinux al contenedor.
--name cursolinux: Nombra el contenedor cursolinux.
--privileged: Otorga privilegios extendidos al contenedor.
jesusimagina/sdk-arm:1.0: Imagen Docker a usar (sdk-imx8 con la etiqueta 1.0).
/bin/bash: Comando a ejecutar dentro del contenedor (una shell Bash).


# 2 - Instalamos dependencias

	apt install bc libgnutls28-dev uuid-dev libssl-dev u-boot-tools device-tree-compiler python3-setuptools swig python3-dev qemu-system-arm

libgnutls28-dev: Librerías de desarrollo para GnuTLS.
uuid-dev: Librerías y archivos de desarrollo para UUID.
libssl-dev: Librerías de desarrollo para OpenSSL.
u-boot-tools: Herramientas para el gestor de arranque U-Boot.
device-tree-compiler: Compilador para Device Tree.
python3-setuptools: Herramientas para empaquetar y distribuir software en Python.
swig: Generador de interfaces para conectar C/C++ con otros lenguajes.
python3-dev: Archivos de desarrollo para Python 3.

# 3 - Descargamos el u-boot

	git clone --depth 1 https://github.com/u-boot/u-boot.git

# 4 - Exportamos el path de la toolchain

	export TOOLCHAIN=/opt/path2toolchain/arm-linux-gnueabihf-

# 5 - Generamos la configuración

	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C u-boot qemu_arm_defconfig O=/work/compile/u-boot-qemu

# 6 - Compilamos

	make -j $(nproc) ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C compile/u-boot-qemu

# 7 - Probamos con qemu

Si hay problemas en el docker se puede hacer desde el host añadiendo *sudo*.
Nos vamos al directorio de la compilación donde encontraremos el *u-boot.bin*:

	qemu-system-arm -machine virt -nographic -bios u-boot.bin

# 8 - Modificamos el tiempo del boot en el menuconfig

	make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C compile/u-boot-qemu menuconfig

# 9 - Probamos a modificar las variables de entorno en la consola del u-boot

	setenv bootdelay 3
	saveenv
	reset

# 10 - Escribir un comando para el u-boot

# 11 - Añadir un argumento (o varios) al comando

# 12 - Escribir un script para ejecutar varios comandos aceptando diversos argumentos

# 13 - Añadir al menuconfig la opción de que esté o no el comando que hemos creado
