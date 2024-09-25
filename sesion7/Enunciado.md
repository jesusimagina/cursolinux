# Ejecicio

Queremos actualizar el kernel de nuestro sistema, para ello vamos a generar una imagen zImage y la vamos a cifrar y firmar.
Luego la llevaremos a nuestro sistema (qemuarm) y allí se descifrará y se verificará la firma.
En nuestro sistema tendremos un módulo de seguridad con el que nos comunicaremos mediante un driver de caracteres
Este módulo tendrá el nodo en /dev/hsm y guardará el certificado de la CA y la clave de cifrado.

Por tanto:

### 1 - La CA y el certificado de usuario que ha firmado lo pasaré yo asi como la clave de cifrado

### 2 - Hacer un driver que tenga las funciones disponibles para obtener la CA y la clave de firma cuando sea necesario

### 3 - Integrar en yocto el código necesario visto ayer para poder descifrar y verificar firmas como una libreria

### 4 - Hacer una aplicación que pueda verificar la firma y descifrar el zImage, para ello:

 - Tiene que poder comunicarse con el driver para obtener la CA y la clave
 - Tiene que tener enlazada la librería crypto que hemos usado
 - Tiene que poder obtener por la entrada el parámetro de zImage.enc.sign


