# Ejecicio

Queremos actualizar el kernel de nuestro sistema, para ello vamos a generar una imagen zImage y la vamos firmar.
Luego la llevaremos a nuestro sistema (qemuarm) y allí se descifrará y se verificará la firma.
En nuestro sistema tendremos un módulo de seguridad con el que nos comunicaremos mediante un driver de caracteres
Este módulo tendrá el nodo en /dev/hsm y guardará el certificado de la CA y la clave de cifrado.

Por tanto:

### 1 - Generar una CA y un certificado de usuario. (Se supone que ya las tenemos)

 - Firmar el zImage

### 2 - Hacer un driver integrado en yocto que tenga las funciones disponibles para obtener la CA. 

La podemos guardar en el propio fichero como un array de bytes. Si al fichero del certificado de la CA le pasamos el comando xxd -i

	xxd -i <nombre_del_certificado>
	
Recordad que lo haremos en formato DER

### 3 - Integrar en yocto el código necesario visto ayer para poder verificar firmas como una libreria

Os pasaré el código yo. La generación de la librería en yocto lo hareís vosotros

### 4 - Hacer una aplicación que pueda verificar la firma del zImage, para ello:

 - Tiene que poder comunicarse con el driver para obtener la CA
 - Tiene que tener enlazada la librería crypto que hemos generado
 - Se le pasará como argumento el zImage, la firma y el certificado de usuario correspondiente a la clave que ha firmado
 
### 5 - Test

La prueba consistirá en pasarle el zImage, al firma y el certificado por scp al qemuarm y ejecutar la aplicación que lo verifique. 


