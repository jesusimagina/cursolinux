# Generar el sdk con el proyecto yocto

Arrancamos el docker en el directorio de trabajo de yocto

	docker run -it --rm  -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux  --privileged -u user ubuntu-poky:latest /bin/bash
	
Hacemos el source para preparar el entorno

	source poky/oe-init-build-env build-qemuarm
	
Generamos la toolchain

	bitbake -c populate_sdk core-image-minimal
	
Una vez generada tendremos en tmp/deploy/sdk un script que tendra un nombre parecido a esto:

	poky-<c_library>-<host_machine>-<target_image><target_machine>-toolchain-<version>.sh
	
Ejecutar ese script nos instalará la toolchain en el directorio que le digamos. Una buena práctica es instalarlo dentro de un docker que se utilice solo para ese fin


### Compilar con el sdk

Para compilar debemos preparar con un script el entorno de compilación:

	source /opt/poky/<version>/environment-setup-<target_machine>-<target_image>
	
Podemos comprobar que la variable de entorno de CC ha cambiado y apunta a esa toolchain

	
### Compilamos sqlite

Las rutas siguientes estan pensadas para usar el docker donde se ha instalado la toolchain

### 1.2 Variables de entorno

export SOURCES=/work/sources
export SQLITE_DIR=$SOURCES/sqlite-snapshot-202408161851
export BUILDDIR=/work/build-sqlite-$TARGET

Podemos descubrir el target haciendo

	$CC -v
	
Nos saldrán los parámetros de configuración del GCC entre ellos el target. Lo podemos copiar y exportar

	export TARGET=<target>
	
### Configuramos y compilamos igual que con nuestro sdk

	mkdir -p $BUILDDIR/sqlite
	pushd $BUILDDIR/sqlite

	$SQLITE_DIR/configure --host=$TARGET --prefix=/usr

	make -j$(nproc)
	make DESTDIR=$($CC -print-sysroot) install

	popd

