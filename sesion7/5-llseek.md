# lseek

En el driver habrá que implementar el llseek

	static loff_t hellokmllseek (struct file *filp, loff_t offset, int whence){
		loff_t newpos = -EINVAL;
		switch( whence ){
		    case SEEK_SET:/* relative from the beginning of file */
			newpos = offset; /* offset become the new position */
			break;
		    case SEEK_CUR: /* relative to current file position */
			/* just add offset to the current position */
			newpos = filp->f_pos + offset;
			break;
		    case SEEK_END: /* relative to end of file */
			newpos = message_size + offset;
			break;
		    default:
		    	return -EINVAL;
		}
		/* Check whether newpos is valid **/
		if ( newpos < 0 )
			return -EINVAL;
		/* Update f_pos with the new position */
		filp->f_pos = newpos;
		/* Return the new file-pointer position */
		return newpos;

	}
	
Si no reconoce lso códigos de error incluir

	#include <linux/errno.h>
	
En el consumidor antes de leer cuando haya datos disponibles incluiremos:

	if (lseek(fd, 0, SEEK_SET) < 0) {
	perror("Failed to seek to the beginning of the file");
	break;
	}
