
# Enlazando módulos

### Hacemos un segundo fichero

Podemos llamarlo loggerkm por ejemplo.

Le añadimos una variable global

	int loggerCount = 0;

Le añadimos una función logMessage

	int logMessage(const char * message)
	{
        	printk(KERN_INFO "Mensaje %d: %s",loggerCount, message);
        	return 0;
	}

El hellokm.c tendrá:

	extern int loggerCount;
	extern int logMessage(const char * message);

En la función init se le puede añadir que llame a logMessage con un mensaje 

### Makefile 

El Makefile anterior 

	
	obj-m += $(MODULE)_lkm.o
	hellokm_lkm-objs := $(MODULE).o loggerkm.o

# Stacking

Convertimos el loggerkm en un módulo a parte a parte de lo necesario para que sea un módulo independiente, añadimos al fichero:

	EXPORT_SYMBOL(loggerCount);
	EXPORT_SYMBOL(logMessage);

El Makefile del hellokm debe modificarse:

	obj-m := $(MODULE).o ../loggerkm/loggerkm.o


