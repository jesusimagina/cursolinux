# Ioctl

Primero hay que implementar las macros con los IOCTL

Editamos el fichero en la receta del modulo: hellokm.h

	#ifndef CURSO_IOCTL_H
	#define CURSO_IOCTL_H

	#define HELLOKM_GETSIZE_CMD 1
	#define HELLOKM_WRITE_CMD 2

	#define HELLOKM_GETSIZE _IOR('h', HELLOKM_GETSIZE_CMD, int *)
	#define IOCTL_WRITE_MSG _IOW('h', HELLOKM_WRITE_CMD, char *)

	#endif 

El driver puede ser así:

	static long hellokm_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	    char user_msg[256];

	    switch (cmd) {
		case HELLOKM_GETSIZE:
		    if (copy_to_user((int *)arg, &message_size, sizeof(message_size))) {
			return -EFAULT;
		    }
		    break;
		case IOCTL_WRITE_MSG:
		    if (copy_from_user(user_msg, (char *)arg, sizeof(user_msg))) {
			return -EFAULT;
		    }
		    strncpy(message, user_msg, sizeof(message));
		    message[sizeof(message) - 1] = '\0'; // Ensure null-termination
		    break;
		default:
		    return -ENOTTY;
	    }
	    return 0;
	}
	
	
A la estructura de operaciones se debe añadir de este modo

	struct file_operations hellokm_fops = {
		   .owner = THIS_MODULE,
		   .open = hellokm_open,
		   .release = hellokm_release,
		   .read = hellokm_read,
		   .write = hellokm_write,
		   .poll = hellokm_poll,
		   .llseek = hellokmllseek,
		   .unlocked_ioctl = hellokm_ioctl,
	};

El consumidor puede añadir esta función antes de leer

	int messagesize = 0;
	if (ioctl(fd, HELLOKM_GETSIZE, &messagesize) < 0) {
		perror("Failed to read message from the device");
		close(fd);
		return EXIT_FAILURE;
	}
	
Debera incluir el hellokm.h por lo que se deberá modificar la recetas de la manera necesaria para que lo encuentre
		    
