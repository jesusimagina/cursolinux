# Solución al ejercicio

### Firma del binario

Con el certificado de usuario de la PKI generada en la sesión 6 podemos firmar el bianrio zImage que encontramos en el directorio de yocto build-qemuarm/tmp/deploy/images/qemuarm/

	openssl dgst -sha512 -sign user-key.pem -out signature zImage
	
Guardamos la firma *signature*, la imagen *zImage* y el *user-cert.der*

Para probar, adjunto mi firma, imagen y certificado de usuario

### Añadimos al capa meta-secureboot de este repositorio a nuestro proyecto

En la capa hay tres recetas:

 - hsmkm: el driver que tiene integrado la CA (en este caso la mía)
 - libcursocrypto: librería crypto que contiene las funciones *verifyCert* y *verify_signature*
 - secboot: aplicación usada para verificar la firma de zImage y que depende de las otras dos
 
Generamos las recetas

	bitbake hsmkm libcursocrypto secboot
	
Las integramos en la imagen con el conf/local.conf

	MACHINE_ESSENTIAL_EXTRA_RDEPENDS += "kernel-module-hsmkm"
	KERNEL_MODULE_AUTOLOAD += "hsmkm"
	IMAGE_INSTALL_append = " secboot"
	
### Construimos la imagen 

	bitbake core-image-minimal
	
Arrancamos el runqemu

	runqemu qemuarm nographic

### Prueba

Con el runquemu arrancado le pasamos por scp los archivos *signature*, la imagen *zImage* y el *user-cert.der*

	scp signature zImage user-cert.der root@<ip-qemu>:/tmp
	
Probamos el secboot

	cd /tmp
	secboot user-cert.der zImage signature

