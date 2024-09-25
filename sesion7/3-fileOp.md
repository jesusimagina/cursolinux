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

	    printk(KERN_INFO "hellokm: Recibido %zu bytes. is_message_available=%d \n", length, is_message_available);
	    return length;
	}
	
### Creamos un una aplicación en espacio de usuario

Creamos la receta
La aplicación debe abrir el dispositivo y escribir lo que se le pase como parámetro de entrada


