# Examen

Tenemos este código que escribe en el dispositivo /dev/hellokm un mensaje que le pasas como parámetro:

	#include <iostream>
	#include <fstream>
	#include <cstring>

	int main(int argc, char* argv[]) {
	    if (argc != 2) {
		std::cerr << "Uso: " << argv[0] << " <mensaje>" << std::endl;
		return 1;
	    }

	    std::ofstream device("/dev/hellokm");
	    if (!device.is_open()) {
		std::cerr << "No se pudo abrir el dispositivo /dev/hellokm" << std::endl;
		return 1;
	    }

	    device << argv[1];
	    device.close();

	    return 0;
	}

#### 1 - Implementa un driver para que guarde el mensaje recibido y que cuando un consumidor lo lea se imprima por pantalla

Nota: El consumidor no hace falta implementarlo. 

	Puede hacerse con cat /dev/hellokm

	
#### 2 - La lectura debe dar error si no hay datos disponibles


#### 3 - Sabrias hacer un consumidor que haga polling para driver

