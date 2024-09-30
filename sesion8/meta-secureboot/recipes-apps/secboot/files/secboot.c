//Includes sistema
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

//Includes driver
#include "hsmkm.h"

//Includes libcursocrypto
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>

#include "common.h"
#include "linuxCrypt.h"

//Nombre dispositivo
#define DEVICE "/dev/hsmkm"


/**
* Función auxiliar para imprimir una caddena de bytes
*
*/
void hexdump(unsigned char * buffer, int len, const char * name){

    printf("%s (%d):", name, len);
    for(int i=0; i<len; i++)
    {
        if(i%16==0 && i !=0)
            printf("\n");

        printf("%X ", buffer[i]);
    }
    printf("\n");
}




int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <user_cert> <zImage> <signature> \n", argv[0]);
        return 1;
    }

    int fd = open(DEVICE, O_RDONLY);

    if (fd < 0) {
        perror("Error al abrir el dispositivo");
        return 1;
    }
    
    //Obtenemos al CA
    int ca_size = 0;
    unsigned char * ca_cert = NULL;
    
    if (ioctl(fd, IOCTL_HSMKM_GETCASIZE, &ca_size) < 0) {
	printf("Error al leer la longitud de la CA");
	close(fd);
	return EXIT_FAILURE;
    }
    
    if(ca_size <= 0){
    	printf("La longitud de la CA es incorrecta %d", ca_size);
	close(fd);
	return EXIT_FAILURE;
    }
    
    ca_cert = malloc(ca_size);
    
    if (ioctl(fd, IOCTL_HSMKM_GETCA, ca_cert) < 0) {
	perror("Error al leer la longitud de la CA");
	close(fd);
	return EXIT_FAILURE;
    }

    close(fd);
    hexdump(ca_cert, ca_size, "CA");
    
    // Verificamos los certificados
    int user_size = 0;
    unsigned char * user_cert = NULL;
    
    user_cert = file_to_unsigned_char(argv[1], &user_size);
    
    verifyCert(ca_cert, ca_size, user_cert, user_size);
    
    
    // Verificamos la imagen
    
    int datalen = 0; 
    int signLen = 0;
    unsigned char * data = file_to_unsigned_char(argv[2], &datalen);
    unsigned char * signature = file_to_unsigned_char(argv[3], &signLen);
     
    if (verify_signature(user_cert, user_size, data,datalen, signature, signLen)) {
        printf("Firma verificada\n");
    } else {
        printf("Firma inválida\n");
    }
    
    free(signature);
    free(data);
    free(ca_cert);
    free(user_cert);
    
    return 0;
    
}
