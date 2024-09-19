### Character Driver

### Entorno

Podemos probarlo en cruzado o en nuestro sistema.

### Creamos un módulo

Podemos evolucionar el propio hellokm.c

### Nuevos includes

Los nuevos includes que nos aportar el nuevo "framework"

	#include <linux/kernel.h>
	#include <linux/module.h>
	#include <linux/init.h>
	#include <linux/fs.h>
	#include <linux/device.h>

### Descripción del dispositivo

	#define DEVICE_NAME "hellokm"
	#define MAJOR_NUM 42
	#define NUM_DEVICES 4

### Operaciones básicas

	static int hellokm_open(struct inode *inode, struct file *file);
	static int hellokm_release(struct inode *inode, struct file *file);
	static ssize_t hellokm_read(struct file *file, char *buffer, size_t length, loff_t * offset);
	static ssize_t hellokm_write(struct file *file, const char *buffer, size_t length, loff_t * offset);

Se deben implementar por ejemplo añadiendo esta línea al cuerpo de la función:

	printk (KERN_INFO "%s\n", __func__);

Y el valor de retorno **IMPORTANTE**

Y las asignamos a la estructura de operaciones:

	struct file_operations hellokm_fops = {
	   .owner = THIS_MODULE,
	   .open = hellokm_open,
	   .release = hellokm_release,
	   .read = hellokm_read,
	   .write = hellokm_write,
	};

### Funciones init/exit

Registramos el dispositivo


	int __init hellokm_init(void)
	{
		int ret;
		int i;
		printk(KERN_INFO "hellokm loaded\n");
		ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &hellokm_fops);
		if (ret != 0)
		{
			printk (KERN_ERR "Error al cargar el módulo\n");
			return ret;
		}
	      
		printk (KERN_INFO "I was assigned major number %d. To talk to\n", MAJOR_NUM);
		printk (KERN_INFO "the driver, create a dev file with\n");
		printk (KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, MAJOR_NUM);
		printk (KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
			printk (KERN_INFO "the device file.\n");
		printk (KERN_INFO "Remove the device file and module when done.\n");
		return 0;
	}

	void __exit hellokm_exit(void)
	{
		unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
		printk("hellokm unloaded\n");
	}



Compilamos e insertamos el módulo, leemos atentamente lo que nos dice los mensajes del kernel.

### Probamos

	cat /dev/hellokm 
	echo "Hola" > /dev/hellokm


