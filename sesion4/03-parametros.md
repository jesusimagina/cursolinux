### Parámetros en lso módulos

En el hellokm.c añadimos una variable de tipo static

	static int mp_debug_level = 0;

Se añade la macro para configurarlo como parámetro del módulo:

	module_param(mp_debug_level, int, 0660);

Se añaden las macros para la descripción que se verá:

	MODULE_PARM_DESC(mp_debug_level, "Debug level [0-2]; 0 => no debug messages, 2 => high verbosity");


En el módulo de inicio hacer algo con ese parámetro.

### Probarlo

	sudo insmod hellokm.ko mp_debug_Level=2
