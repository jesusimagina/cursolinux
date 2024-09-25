# Modos de creación de nodos

### 1 - "A mano"

Editamos el hellokm.c

	#include <linux/kernel.h>
	#include <linux/module.h>
	#include <linux/init.h>
	#include <linux/fs.h>
	#include <linux/device.h>

	#define DEVICE_NAME "hellokm"
	#define MAJOR_NUM 42
	#define NUM_DEVICES 4

	MODULE_AUTHOR("SEnSEi");
	MODULE_DESCRIPTION("Driver Básico");
	MODULE_LICENSE("GPL");
	MODULE_VERSION("1.0");

	#define DEVICE_NAME "hellokm"
	#define MAJOR_NUM 42
	#define NUM_DEVICES 4

	static int hellokm_open(struct inode *inode, struct file *file);
	static int hellokm_release(struct inode *inode, struct file *file);
	static ssize_t hellokm_read(struct file *file, char *buffer, size_t length, loff_t * offset);
	static ssize_t hellokm_write(struct file *file, const char *buffer, size_t length, loff_t * offset);


	struct file_operations hellokm_fops = {
		   .owner = THIS_MODULE,
		   .open = hellokm_open,
		   .release = hellokm_release,
		   .read = hellokm_read,
		   .write = hellokm_write,
	};

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


	static int hellokm_open(struct inode *inode, struct file *file)
	{
	   printk (KERN_INFO "%s\n", __func__);
	   return 0;
	}

	static int hellokm_release(struct inode *inode, struct file *file)
	{
	   printk (KERN_INFO "%s\n", __func__);
	   return 0;
	}

	static ssize_t hellokm_read(struct file *file,
	 char *buffer, size_t length, loff_t * offset)
	{
	   printk (KERN_INFO "%s %u\n", __func__, length);
	   return 0;
	}

	static ssize_t hellokm_write(struct file *file,
	 const char *buffer, size_t length, loff_t * offset)
	{
	   printk (KERN_INFO "%s %u\n", __func__, length);
	   return length;
	}

	module_init(hellokm_init);
	module_exit(hellokm_exit);
	
### 2 -  Creación del dispositivo automática

Añadimos 

	static struct class *hellokm_class;

En la rutina del init despues del registro:

	hellokm_class = class_create(DEVICE_NAME);
	
	for (i = 0; i < NUM_DEVICES; i++) {
		device_create(hellokm_class, NULL,
		    MKDEV(MAJOR_NUM, i), NULL, "hellokm%d", i);
	}

En el exit antes del desregistro

	for (int i = 0; i < NUM_DEVICES; i++) {
		device_destroy(hellokm_class, MKDEV(MAJOR_NUM, i));
	}
	
	class_destroy(hellokm_class);
	
### 3 - Asignación dinámica de major y minor

Nuevos includes

	#include <linux/cdev.h>

Nuevas estructuras

	static dev_t dev;
	static struct cdev hellokm_cdev;
	
Nuevo define

	#define HELLOKM_CLASS "hellokm-class"
	
Modificamos las funciones de entrada y salida 

	int __init hellokm_init(void)
	{
	  /*Allocating Major number */
	  if ((alloc_chrdev_region (&dev, 0, 1, DEVICE_NAME)) < 0)
	    {
	      printk (KERN_INFO "Cannot allocate major number\n");
	      return -1;
	    }
	  printk (KERN_INFO "Major = %d Minor = %d \n", MAJOR (dev), MINOR (dev));

	  /*Creating cdev structure */
	  cdev_init (&hellokm_cdev, &hellokm_fops);
	  hellokm_cdev.owner = THIS_MODULE;
	  hellokm_cdev.ops = &hellokm_fops;

	  /*Adding character device to the system */
	  if ((cdev_add (&hellokm_cdev, dev, 1)) < 0)
	    {
	      printk (KERN_INFO "Cannot add the device to the system\n");
	      goto r_class;
	    }

	  /*Creating struct class */
	  if ((hellokm_class = class_create (THIS_MODULE, HELLOKM_CLASS)) == NULL)
	    {
	      printk (KERN_INFO "Cannot create the struct class\n");
	      goto r_class;
	    }

	  /*Creating device */
	  if ((device_create (hellokm_class, NULL, dev, NULL, DEVICE_NAME)) == NULL)
	    {
	      printk (KERN_INFO "Cannot create the Device 1\n");
	      goto r_device;
	    }

	  return 0;

	r_device:
	  class_destroy (hellokm_class);
	r_class:
	  unregister_chrdev_region (dev, 1);
	  cdev_del (&hellokm_cdev);
	  return -1;
	}

	void __exit hellokm_exit(void)
	{

	  device_destroy (hellokm_class, dev);
	  class_destroy (hellokm_class);
	  cdev_del (&hellokm_cdev);
	  unregister_chrdev_region (dev, 1);
	  printk (KERN_INFO "Device Driver Remove...Done!!!\n");

	  printk("hellokm unloaded\n");
	}
	

