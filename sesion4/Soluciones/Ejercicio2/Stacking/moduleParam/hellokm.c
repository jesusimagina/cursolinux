#include <linux/init.h>
#include <linux/module.h>

MODULE_AUTHOR("Profesor de SE");
MODULE_DESCRIPTION("El primer modulo que saluda");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

/* Module parameters */
static int mp_debug_level=0;
module_param(mp_debug_level, int, 0660);
static char *mp_strparam = "My string param";
module_param(mp_strparam, charp, 0660);


static int __init hellokm_init(void)
{
	printk(KERN_INFO "Hello World from Kernel\n");
	if(mp_debug_level==2)
	{
	        printk(KERN_INFO "%s\n", (const char*)mp_strparam);
	}
	return 0;
}

static void __exit hellokm_exit(void)
{
	printk(KERN_INFO "Goodbye World\n");
}


/* Module parameters */
MODULE_PARM_DESC(mp_debug_level, "Debug level [0-2]; 0 => no debug messages, 2 => high verbosity");
MODULE_PARM_DESC(mp_strparam, "A demo string parameter");

module_init(hellokm_init);
module_exit(hellokm_exit);




