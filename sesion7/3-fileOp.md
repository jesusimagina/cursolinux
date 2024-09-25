# Operaciones de fichero

### Lectura y escritura

Includes

	#include<linux/uaccess.h>	//copy_to/from_user()
		
Vamos a añadir las variables 

	#define BUFFER_SIZE 32
	static char message[BUFFER_SIZE] = {0};
	static int message_size = 0;

La función de lectura podría ser algo así
	
	static ssize_t hellokm_read(struct file *file,
	 char *buffer, size_t length, loff_t * offset)
	{
	    if (*offset >= message_size) {
		return 0; // End of file
	    }

	    if (length > message_size - *offset) {
		length = message_size - *offset;
	    }

	    printk(KERN_INFO "hellokm read: Leyendo mensaje %s", message);
	    if (copy_to_user(buffer, message, length) != 0) {
		return -EFAULT;
	    }
	   
	    *offset = length;
	    return length;
	}
	
La función write 

	static ssize_t hellokm_write(struct file *file,
	 const char *buffer, size_t length, loff_t * offset)
	{
	    if (length > BUFFER_SIZE - 1) {
		length = BUFFER_SIZE - 1;
	    }

	    if (copy_from_user(message, buffer, length) != 0) {
		return -EFAULT;
	    }

	    message[length] = '\0'; // Null-terminate the string
	    message_size = length;

	    printk(KERN_INFO "hellokm: Recibido %zu bytes. is_message_available=%d \n", length);
	    return length;
	}
	
### Creamos un una aplicación en espacio de usuario

Creamos la receta
La aplicación debe abrir el dispositivo y escribir lo que se le pase como parámetro de entrada

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <fcntl.h>
	#include <unistd.h>
	
	#define MAX_LEN 32
	
	int main(int argc, char *argv[]) {
	    if (argc != 2) {
	        fprintf(stderr, "Uso: %s <mensaje>\n", argv[0]);
	        return 1;
	    }
	
	    if (strlen(argv[1]) > MAX_LEN) {
	        fprintf(stderr, "El mensaje no debe sobrepasar los %d bytes.\n", MAX_LEN);
	        return 1;
	    }
	
	    int fd = open("/dev/hellokm", O_WRONLY);
	    if (fd == -1) {
	        perror("Error al abrir /dev/hellokm");
	        return 1;
	    }
	
	    if (write(fd, argv[1], strlen(argv[1])) == -1) {
	        perror("Error al escribir en /dev/hellokm");
	        close(fd);
	        return 1;
	    }
	
	    close(fd);
	    return 0;
	}



