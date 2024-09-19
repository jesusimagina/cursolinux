#include <linux/init.h>
#include <linux/module.h>

MODULE_AUTHOR("Profesor de SE");
MODULE_DESCRIPTION("Logger para probar dependencias");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

int loggerCount = 0;

int logMessage(const char * message);

EXPORT_SYMBOL(loggerCount);
EXPORT_SYMBOL(logMessage);

static int __init loggerkm_init(void)
{
	printk(KERN_INFO "Cargamos el logerkm\n");
	return 0;
}

static void __exit loggerkm_exit(void)
{
	printk(KERN_INFO "Goodbye logger\n");
}

int logMessage(const char * message)
{
        printk(KERN_INFO "Mensaje %d: %s",loggerCount, message);
        return 0;
}

module_init(loggerkm_init);
module_exit(loggerkm_exit);





