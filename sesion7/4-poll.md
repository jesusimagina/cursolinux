# Operacion de poll

Includes 

	#include <linux/wait.h>
	#include <linux/sched.h>
	#include <linux/poll.h>

Demomento solo haremos que avise si hay algo escrito. Definiremos estas variables
	
	static int is_message_available = 0;
	static DECLARE_WAIT_QUEUE_HEAD(wq);
	
Una funcón poll podria ser

	static unsigned int hellokm_poll(struct file *filep, poll_table *wait) {
	    poll_wait(filep, &wq, wait);
	    unsigned int mask = 0;

	    if (is_message_available) {
		mask |= POLLIN | POLLRDNORM; // Readable
		is_message_available = 0;
	    }

	    return mask;
	}
	
Generar una receta para este fichero consumidor.c que hace un poll sobre el dispositivo y cuando hay datos lee lo que haya

	#include <stdio.h>
	#include <stdlib.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <poll.h>

	#define DEVICE "/dev/hellokm"

	int main() {
	    int fd = open(DEVICE, O_RDONLY);
	    if (fd < 0) {
		perror("Failed to open the device");
		return 1;
	    }

	    struct pollfd fds;
	    fds.fd = fd;
	    fds.events = POLLIN;

	    int ret = poll(&fds, 1, -1); // Infinite timeout
	    if (ret > 0) {
		if (fds.revents & POLLIN) {
		    char buffer[100];
		    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
		    if (bytesRead < 0) {
		        perror("Failed to read from the device");
		    } else {
		        buffer[bytesRead] = '\0';
		        printf("Read from device: %s\n", buffer);
		    }
		}
	    } else {
		perror("Poll error");
	    }

	    close(fd);
	    return 0;
	}
	
	
Despues de probarlo podemos hacer que el consumidor se quede en un bucle constante y lea todos los mensajes
 
