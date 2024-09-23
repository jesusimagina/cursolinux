### Dependencias entre recetas

Vamos a generar una dependencia con la receta que hemos creado para generar el logger
Modificamos el helloworld.c para que quede así

	#include <stdio.h>
	#include "logger.h"

	int main()
	{
	    logger_initConsoleLogger(stderr);
	    logger_setLevel(LogLevel_DEBUG);
	    LOG_INFO("Hello World");
	    return 0;
	}


### Añadimos el append a la receta

Generamos un helloworld_1.0.bbappend

	DEPENDS += "logger"

	do_compile(){
	    ${CC} ${CFLAGS} ${LDFLAGS} helloworld.c -o helloworld -llogger
	}

	RDEPENDS_${PN} = "logger"

### Añadimos a la imagen las dos recetas

	IMAGE_INSTALL_append += " helloworld"
	
### Generamos la imagen

Compilamos de nuevo los paquetes

	bitbake -c cleanall logger helloworld
	bitbake  logger helloworld
	
Si todo ha ido bien generamos la imagen

	bitbake core-image-minimal
	
### Probamos 

	runqemu qemuarm nographic
	
Una vez arranque comprobar el comando helloworld
