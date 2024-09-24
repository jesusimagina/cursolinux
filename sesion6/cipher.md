# Ejercicios para cifrar con openssl 

### 1 - Probamos primero a cifrar algun texto aleatorio

Generamos un fichero de texto text.plain con algún contenido (conocido)

	openssl enc -aes-256-cbc -salt -in text.plain -out text.enc

Podemo sver el resultado de la salida

Desciframos con 

	openssl enc -d -aes-256-cbc -in text.enc -out result.plain


### 2 - Generamos una clave para cifrar

Las claves son números aleatorios. Es importante el concepto de ruido.

	openssl rand -out cipherkey 32
		
Se puede ver la clave con xxd o hexdump

	xxd -p cipherkey
	
Utilizamos los comandos para cifrar y descifrar
		
	openssl enc -aes-256-cbc -salt -in text.plain -out text.enc -pass file:./cipherkey

	openssl enc -d -aes-256-cbc -in text.enc -out result.plain -pass file:./cipherkey	
	
### 3 - Ejemplo por código

Generamos el fichero encrypt.c

A este ejecutable se le pasará dos argumentos de entrada, el texto en claro, y el nombre del fichero cifrado.
La clave la leera por fichero. Se encontrará en el mismo directorio que el ejecutable y se llamará cipherkey.


	#include <openssl/conf.h>
	#include <openssl/evp.h>
	#include <openssl/err.h>
	#include <openssl/rand.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>

	void handleErrors(void) {
	    ERR_print_errors_fp(stderr);
	    abort();
	}

	int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
		    unsigned char *iv, unsigned char *ciphertext) {
	    EVP_CIPHER_CTX *ctx;
	    int len;
	    int ciphertext_len;

	    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	    ciphertext_len = len;

	    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	    ciphertext_len += len;

	    EVP_CIPHER_CTX_free(ctx);

	    return ciphertext_len;
	}

	int main(int argc, char *argv[]) {
	    if (argc != 3) {
		fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
		return 1;
	    }

	    FILE *keyFile = fopen("cipherkey", "rb");
	    if (!keyFile) {
		perror("Key file error");
		return 1;
	    }
	    unsigned char key[32]; // 256-bit key
	    fread(key, 1, sizeof(key), keyFile);
	    fclose(keyFile);

	    unsigned char iv[16];
	    if (!RAND_bytes(iv, sizeof(iv))) handleErrors();

	    FILE *inputFile = fopen(argv[1], "rb");
	    FILE *outputFile = fopen(argv[2], "wb");
	    if (!inputFile || !outputFile) {
		perror("File error");
		return 1;
	    }

	    fseek(inputFile, 0, SEEK_END);
	    long inputFileLen = ftell(inputFile);
	    fseek(inputFile, 0, SEEK_SET);

	    unsigned char *plaintext = malloc(inputFileLen);
	    fread(plaintext, 1, inputFileLen, inputFile);

	    unsigned char ciphertext[inputFileLen + EVP_MAX_BLOCK_LENGTH];
	    int ciphertext_len = encrypt(plaintext, inputFileLen, key, iv, ciphertext);

	    fwrite(iv, 1, sizeof(iv), outputFile); // Write IV to output file
	    fwrite(ciphertext, 1, ciphertext_len, outputFile);

	    fclose(inputFile);
	    fclose(outputFile);
	    free(plaintext);

	    return 0;
	}

Compilamos usando el propio gcc enlazando las librerias de openssl

	gcc -o encrypt encrypt.c -lssl -lcrypto

Se puede probar el fichero de antes

	./encrypt text.plain text.enc

### 4 - Decrypt

Hacemos algo similar pero para descifrar

	#include <openssl/conf.h>
	#include <openssl/evp.h>
	#include <openssl/err.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>

	void handleErrors(void) {
	    ERR_print_errors_fp(stderr);
	    abort();
	}

	int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
		    unsigned char *iv, unsigned char *plaintext) {
	    EVP_CIPHER_CTX *ctx;
	    int len;
	    int plaintext_len;

	    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	    plaintext_len = len;

	    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
	    plaintext_len += len;

	    EVP_CIPHER_CTX_free(ctx);

	    return plaintext_len;
	}

	int main(int argc, char *argv[]) {
	    if (argc != 3) {
		fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
		return 1;
	    }

	    FILE *keyFile = fopen("cipherkey", "rb");
	    if (!keyFile) {
		perror("Key file error");
		return 1;
	    }
	    unsigned char key[32]; // 256-bit key
	    fread(key, 1, sizeof(key), keyFile);
	    fclose(keyFile);

	    unsigned char iv[16];

	    FILE *inputFile = fopen(argv[1], "rb");
	    FILE *outputFile = fopen(argv[2], "wb");
	    if (!inputFile || !outputFile) {
		perror("File error");
		return 1;
	    }

	    fread(iv, 1, sizeof(iv), inputFile); // Read IV from input file

	    fseek(inputFile, 0, SEEK_END);
	    long inputFileLen = ftell(inputFile) - sizeof(iv);
	    fseek(inputFile, sizeof(iv), SEEK_SET);

	    unsigned char *ciphertext = malloc(inputFileLen);
	    fread(ciphertext, 1, inputFileLen, inputFile);

	    unsigned char plaintext[inputFileLen];
	    int plaintext_len = decrypt(ciphertext, inputFileLen, key, iv, plaintext);

	    fwrite(plaintext, 1, plaintext_len, outputFile);

	    fclose(inputFile);
	    fclose(outputFile);
	    free(ciphertext);

	    return 0;
	}
	
Compilamos usando el propio gcc enlazando las librerias de openssl

	gcc -o decrypt decrypt.c -lssl -lcrypto

Si no encuentra las cabeceras es posible que haya que instalar el paquete de desarrollo

	sudo apt install libssl-dev

Se puede probar el fichero de antes

	./decrypt text.enc text.plain
