

### 10 - Comando hello

Crear fichero *cmd/hello_world.c*

	#include <command.h>

	#ifdef CONFIG_CMD_HELLO
	
	static int do_hello(struct cmd_tbl *cmdtp, int flag, int argc,
			   char *const argv[])
	{
	    if(argc > 1)
	    {
		printf("Hello %s from u-boot\n", (const char*)argv[1]);
	    }
	    else
	    {
		printf("Hello from u-boot\n");
	    }
		return 0;
	}

	U_BOOT_CMD(
		hello,	CONFIG_SYS_MAXARGS,	1,	do_hello,
		"print command description/usage",
		"\n"
		"	- Saluda al mundo  desde el u-boot\n"
		"hello command ...\n"
		"	- print detailed usage of 'command'"
	);
	
	#endif
	
### El makefile

Editar el makefile *cmd/Makefile*

	obj-$(CONFIG_CMD_HELLO) += hello_world.o
	
### El kbuild

Editar el */cmd/Kconfig*

	 if CMDLINE
	 
	config CMD_HELLO
	       bool "Use hello command"
	       help
	         This option enables the "hello" command.
	         default n
	
	config HUSH_PARSER
		bool "Use hush shell"
		help

# Script en u-boot

setenv hello "hello;hello 2"
