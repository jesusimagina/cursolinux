# Modificaciones a nuestro driver

### 1 - Creación del dispositivo automática

Añadimos 

	static struct class *hellokm_class;

En la rutina del init despues del registro:

	hellokm_class = class_create(DEVICE_NAME);
	
	for (int i = 0; i < NUM_DEVICES; i++) {
		device_create(hellokm_class, NULL,
		    MKDEV(MAJOR_NUM, i), NULL, "hellokm%d", i);
	}

En el exit antes del desregistro

	for (int i = 0; i < NUM_DEVICES; i++) {
		device_destroy(hellokm_class, MKDEV(MAJOR_NUM, i));
	}
	
	class_destroy(hellokm_class);


### Asignación dinámica

Nuevos includes

	#include <linux/cdev.h>

Nuevas estructuras

	static dev_t dev;
	static struct cdev hellokm_cdev;


### Funciones de entrada salida

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
	  if ((hellokm_class = class_create (HELLOKM_CLASS)) == NULL)
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
	



