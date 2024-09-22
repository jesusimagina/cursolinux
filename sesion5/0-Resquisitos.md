# Requisistos para la sesión 5

### Descargar la precompilación

En un directorio de trabajo limpio:

	mkdir build-qemuarm

Cuando compartamos este directorio con el docker la idea es que el arbol de directorios quede así (poky lo descargaremos durante la sesión):

	/work/
	.
	|-- build-qemuarm
	`-- poky

Si no se va a usar el docker se debe generar el directorio de trabajo dentro del sistema, hay que instalar las dependencias que hay en el fichero 0-crearDocker.md obviando la parte de usuario y generar el directorio de trabajo en /work asignando los permisos adecuados.

### Descomprimir

Descargamos dentro de **build-qemuarm** el paquete generado y lo descomprimimos:

	pigz -d -k -p $(nproc) build.tar.gz
	tar -xvf build.tar

Esto puede tardar un rato porque son varios GB.

Una vez descomprimido ya estaría listo

