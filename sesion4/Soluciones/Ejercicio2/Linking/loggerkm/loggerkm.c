#include <linux/init.h>
#include <linux/module.h>


int loggerCount = 0;

int logMessage(const char * message);

int logMessage(const char * message)
{
        printk(KERN_INFO "Mensaje %d: %s",loggerCount, message);
        return 0;
}






