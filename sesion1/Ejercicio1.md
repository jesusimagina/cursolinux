# Ejercicios tema toolchain

[TOC]

## 1 - Preparar el entorno:

Vamos a utilizar un docker para la compilaciíon: ubuntu:22.04

    docker pull ubuntu:22.04
    docker run -it --rm -v $(pwd):/work -v $(pwd)/install:/opt --hostname cursolinux --name cursolinux  ubuntu:22.04 /bin/bash

### 1.1 Actualizamos el docker y generamos una versión

Actualizamos el docker e instalamos las dependencias de la toolchain:

    apt update
    apt upgrade -y
    apt-get install -y build-essential flex bison texinfo gawk python3 rsync file pkg-config wget

Podríamos instalar también las dependecias de gcc: libgmp3-dev libmpc-dev libmpfr-dev. Pero por ahora las compilaremos a mano.

Explicación de las dependencias instaladas:

- build-essential: incluye herramientas básicas de compilación (gcc, g++, make, etc.).
- flex: Generador de analizadores léxicos.
- bison: Generador de analizadores sintácticos.
- texinfo: Sistema de documentación, genera manuales en varios formatos.
- gawk: Implementación de AWK, un lenguaje de procesamiento de texto.
- python3: Intérprete de Python 3.
- rsync: Herramienta para sincronización de archivos.
- file: Determina el tipo de un archivo.
- wget: para descargar archivos de la web.

Generamos una versión del docker y la renombramos como queramos. Desde el host:

    docker commit cursolinux ubuntu-dev:toolchain

Si hacemos `docker images` veremos:

    REPOSITORY                   TAG                   IMAGE ID       CREATED         SIZE
    ubuntu-dev                   toolchain             aa82bc84f869   4 seconds ago   500MB

## 2 - Descargamos los fuentes

Los fuentes que queremos usar.

- binutils 2.43
- gcc 12.2.0
- glibc 2.37s
- gmp 6.2.1
- mpc 1.2.1
- mpfr 4.1.0
- kernel 6.10.7

## 3 - Compilamos la toolchain

### 3.1 - Variables de entorno


 - PREFIX: Directorio de instalación (dentro de sistema de archivos del docker):
 - TARGET: prefijo del nombre de los binarios, en x86 no es necesario pero por distinguirlos puede ser util.
 - ARCH: arquitectura para la que se va a compilar
 - Directorio de los fuentes

    export SOURCES=/work/sources
    export TARGET=arm-linux-gnueabihf
    export PREFIX=/opt/sdk-$TARGET
    export ARCH=arm
    export BINUTILS_DIR=$SOURCES/binutils-2.43
    export GCC_DIR=$SOURCES/gcc-12.2.0
    export GMP_DIR=$SOURCES/gmp-6.2.1 
    export MPFR_DIR=$SOURCES/mpfr-4.1.0
    export MPC_DIR=$SOURCES/mpc-1.2.1
    export GLIBC_DIR=$SOURCES/glibc-2.37
    export KERNEL_SRC=$SOURCES/linux-6.10.7
    export BUILDDIR=/work/build-sdk-$TARGET

### 3.2 - Compilamos Binutils

La opción `--with-fpu=vfp` y `--with-float=hard` habilitan la compilación para hard float
La opción `--disable-multilib` desactiva la generación de bibliotecas para múltiples arquitecturas (multilib).  

    mkdir -p $BUILDDIR/binutils

    pushd $BUILDDIR/binutils

    $BINUTILS_DIR/configure \
                            --prefix=$PREFIX \
                            --with-sysroot=$PREFIX/$TARGET/sys-root \
                            --target=$TARGET \
                            --with-arch=$ARCH \
                            --with-fpu=vfp \
                            --with-float=hard \
                            --disable-multilib \
                            --enable-shared \
                            --disable-nls

    make -j$(nproc)
    make install
    popd

El resultado de la compilación es el siguiente:

    build-sdk-arm-linux-gnueabihf
    ├── arm-linux-gnueabihf       # Herramientas y archivos específicos para la arquitectura ARM.
    │   ├── bin                   # Binarios específicos para ARM.
    │   └── lib                   # Bibliotecas específicas para ARM.
    │       └── ldscripts         # Scripts de enlazado específicos para ARM.
    ├── bin                       # Binarios generales de binutils.
    ├── include                   # Archivos de cabecera (headers) generales.
    ├── lib                       # Bibliotecas generales.
    │   └── bfd-plugins           # Plugins para el BFD (Binary File Descriptor library).
    ├── share                     # Archivos compartidos de datos y documentación.
    │   ├── info                  # Archivos de documentación en formato info.
    │   ├── locale                # Archivos de localización para distintos idiomas.
    │   │   ├── <language_code>   # Directorios para cada idioma.
    │   │   │   └── LC_MESSAGES   # Archivos de mensajes de localización.
    │   └── man                   # Páginas de manual.
    │       └── man1              # Páginas de manual de la sección 1.
    └── x86_64-pc-linux-gnu       # Herramientas y archivos específicos para la arquitectura x86_64.
        └── arm-linux-gnueabihf   # Subdirectorio específico para ARM dentro de x86_64.
            ├── include           # Archivos de cabecera específicos para ARM.
            └── lib               # Bibliotecas específicas para ARM.


### 3.3 - Compilamos las dependencias de GCC

Las dependencias se compilan para la arquitectura del host, porque gcc hace uso de ellas donde se ejecuta

#### 3.3.1 GMP

    mkdir -p $BUILDDIR/gmp
    pushd $BUILDDIR/gmp
    $GMP_DIR/configure --prefix=$PREFIX
    make -j$(nproc)
    make install DESTDIR=$INSTALLDIR
    popd

#### 3.3.2 MPFR

    mkdir -p $BUILDDIR/mpfr
    pushd $BUILDDIR/mpfr
    $MPFR_DIR/configure --prefix=$PREFIX --with-gmp-include=$PREFIX/include --with-gmp-lib=$PREFIX/lib
    make -j$(nproc)
    make install DESTDIR=$INSTALLDIR
    popd

#### 3.3.3 MPC

    mkdir -p $BUILDDIR/mpc
    pushd $BUILDDIR/mpc
    $MPC_DIR/configure --prefix=$PREFIX --with-gmp-include=$PREFIX/include --with-gmp-lib=$PREFIX/lib
    make -j$(nproc)
    make install DESTDIR=$INSTALLDIR
    popd

El resultado de la compilación será:

    build-sdk-arm-linux-gnueabihf
    ├──include
    │   ├── gmp.h
    │   ├── mpf2mpfr.h
    │   ├── mpfr.h
    ├──lib 
    │   ├── libgmp.a
    │   ├── libgmp.la
    │   ├── libgmp.so -> libgmp.so.10.4.1
    │   ├── libgmp.so.10 -> libgmp.so.10.4.1
    │   ├── libgmp.so.10.4.1
    │   ├── libmpfr.a
    │   ├── libmpfr.la
    │   ├── libmpfr.so -> libmpfr.so.6.1.0
    │   ├── libmpfr.so.6 -> libmpfr.so.6.1.0
    │   ├── libmpfr.so.6.1.0
    │   ├──pkconfig
    │       ├── gmp.pc
    │       └── mpfr.pc
    ├──share
    │   ├──share 
    │   │   └── mpfr
    │   ├──info
    │   │   ├── gmp.info
    │   │   ├── gmp.info-1
    │   │   ├── gmp.info-2
    │   │   ├── mpfr.info

### 3.4 - Linux kernel headers

Para instalar los headers simplemente ejecutar:

    make ARCH=$ARCH -C $KERNEL_SRC INSTALL_HDR_PATH=$PREFIX/$TARGET/sys-root/usr headers_install

El resultado:

    ├── arm-linux-gnueabihf
    │   ├── bin
    │   ├── lib
    │   │   └── ldscripts
    │   └── sys-root
    │       └── usr
    │           └── include
    │               ├── asm
    │               ├── asm-generic
    │               ├── drm
    │               ├── linux
    │               │   ├── android
    │               │   ├── byteorder
    │               │   ├── caif
    │               │   ├── can
    │               │   ├── cifs
    │               │   ├── dvb
    │               │   ├── genwqe
    │               │   ├── hdlc
    │               │   ├── hsi
    │               │   ├── iio
    │               │   ├── isdn
    │               │   ├── misc
    │               │   ├── mmc
    │               │   ├── netfilter
    │               │   │   └── ipset
    │               │   ├── netfilter_arp
    │               │   ├── netfilter_bridge
    │               │   ├── netfilter_ipv4
    │               │   ├── netfilter_ipv6
    │               │   ├── nfsd
    │               │   ├── raid
    │               │   ├── sched
    │               │   ├── spi
    │               │   ├── sunrpc
    │               │   ├── surface_aggregator
    │               │   ├── tc_act
    │               │   ├── tc_ematch
    │               │   └── usb
    │               ├── misc
    │               │   └── uacce
    │               ├── mtd
    │               ├── rdma
    │               │   └── hfi
    │               ├── scsi
    │               │   └── fc
    │               ├── sound
    │               │   └── sof
    │               ├── video
    │               └── xen

### 3.5 GCC Primera fase

    mkdir -p $BUILDDIR/gcc
    pushd $BUILDDIR/gcc

    $GCC_DIR/configure --prefix=$PREFIX \
            --target=$TARGET \
            --with-fpu=vfp \
            --with-float=hard \
            --disable-multilib \
            --enable-languages=c \
            --disable-shared \
            --disable-threads \
            --without-headers \
            --disable-libmudflap \
            --disable-nls \
            --with-gmp=$PREFIX/  \
            --with-mpfr=$PREFIX/ \
            --with-mpc=$PREFIX/

    make -j$(nproc) all-gcc
    make install-gcc
    make -j $(nproc) all-target-libgcc
    make -j $(nproc) install-target-libgcc
    popd

En la carpeta:
    build-sdk-arm-linux-gnueabihf
    ├── bin    

deberían estar los compiladores arm-linux-gnueabihf-gcc y cosas de gcc


### 3.6 GLIBC

    mkdir -p $BUILDDIR/glibc
    pushd $BUILDDIR/glibc

    CC=$PREFIX/bin/$TARGET-gcc \
    LD=$PREFIX/bin/$TARGET-ld \
    AR=$PREFIX/bin/$TARGET-ar \
    RANLIB=$PREFIX/bin/$TARGET-ranlib \
    $GLIBC_DIR/configure --prefix=/usr \
                --build=$MACHTYPE \
                --host=$TARGET \
                --target=$TARGET \
                --with-arch=$ARCH \
                --with-headers=$PREFIX/$TARGET/sys-root/usr/include \
                --with-fpu=vfp \
                --with-float=hard \
                --disable-multilib \
                --disable-werror \
                --disable-nls \
                --disable-profile \
                --enable-add-on \
                --with-tls \                
                libc_cv_forced_unwind=yes \
                libc_cv_c_cleanup=yes \
                libc_cv_arm_tls=yes
    
    make \
        install-bootstrap-headers=yes \
        install_root=$PREFIX/$TARGET/sys-root/ \
        install-headers
    make -j$(nproc) all 
    make install_root=$PREFIX/$TARGET/sys-root/ install
    popd

--with-tls: Habilita el soporte para Thread-Local Storage (TLS).
--disable-profile: Desactiva el soporte para la generación de perfiles.
--enable-add-ons: Habilita la compilación de complementos adicionales.
libc_cv_forced_unwind=yes: Fuerza la habilitación del soporte para forced unwind.
libc_cv_c_cleanup=yes: Habilita el soporte para la limpieza de C++ en la biblioteca.
libc_cv_arm_tls=yes: Confirma que TLS está soportado en la arquitectura ARM.
--disable-nls: Desactiva el soporte para la internacionalización (National Language Support).


En $PREFIX/$TARGET/sys-root/ debería estar en lib la libc y el resto de librerías del paquete 


### 3.7 GCC Segunda fase

    mkdir -p $BUILDDIR/gcc-2
    pushd $BUILDDIR/gcc-2

    $GCC_DIR/configure --prefix=$PREFIX \
            --target=$TARGET \
            --with-sysroot=$PREFIX/$TARGET/sys-root \
            --with-fpu=vfp \
            --with-float=hard \
            --disable-multilib \
            --enable-languages=c,c++ \
            --enable-shared \
            --disable-nls \
            --with-gmp=$PREFIX/  \
            --with-mpfr=$PREFIX/ \
            --with-mpc=$PREFIX/

    make -j$(nproc)
    make install
    popd

Ya estaría la toolchain completa con $PREFIX/bin/$TARGET-gcc deberíais porder compilar


### 3.8 Instalamos la toolchain 

    cp -R $PREFIX /opt

    docker commit cursolinux sdk-arm:1.0


### 3.9 Probamos la toolchain

Creamos un directorio test

    mkdir test
    cd test

Creamos un hello.cpp

    #include <iostream>

    int main()
    {
        std::cout << "Hello world" << std::endl;

        return 0;
    }

Arrancamos el docker de la sdk

    docker run -it --rm -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux sdk-arm:1.0 /bin/bash

Compilamos "a mano":

    /opt/sdk-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++ -o hello hello.cpp

    file hello

    hello: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 3.2.0, with debug_info, not stripped


Podemos hacer un Makefile para compilar el test:

    export TOOLCHAIN_PATH=/opt/sdk-arm-linux-gnueabihf/bin/arm-linux-gnueabihf

    all:
        ${TOOLCHAIN_PATH}-g++ -c hello.cpp -o hello.o
        ${TOOLCHAIN_PATH}-g++ hello.cpp -o hello-sta.elf -static
        ${TOOLCHAIN_PATH}-g++ hello.cpp -o hello-dyn.elf
        ${TOOLCHAIN_PATH}-objdump -d hello.o > hello.src

    clean:
        rm -rf *.o *.elf *.src *.out
    
    .PHONY: all clean

### Ejecutamos en un docker de arm

    apt-get install qemu-user-static

    docker run --rm --privileged multiarch/qemu-user-static --reset -p yes

    docker run -it --rm --platform linux/arm/v7 -v $(pwd):/work -w /work --hostname cursolinux --name cursolinux arm32v7/debian /bin/bash

    Probamos los distintos ejecutables
