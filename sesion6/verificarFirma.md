# Verificación de firma de datos

Con openssl también podemos generar firmas a datos pasandole una clave privada y los datos a firmar

	openssl dgst -sha512 -sign private_key.pem -out signature.bin datos.txt
	
Del mismo modo podemos verificarlo con la clave pública.

	openssl dgst -sha512 -verify public_key.pem -signature signature.bin datos.txt

Como la clave pública hemos visto que esta integrada en el fichero de la privada podemos hacer lo siguiente:
	
	openssl ec -in private_key.pem -pubout -out public_key.pem
	openssl ec -in private_key.pem -pubout -out public_key.pem



### 2 - Programa de verificación de firma

Podemos generar un verifySignature.c

	#include <openssl/evp.h>
	#include <openssl/pem.h>
	#include <openssl/err.h>
	#include <openssl/x509.h>
	#include <stdio.h>
	#include <stdlib.h>

	void handleErrors() {
	    ERR_print_errors_fp(stderr);
	    abort();
	}

	int verify_signature(const char *cert_file, const char *data_file, const char *sig_file) {
	    FILE *fp;
	    X509 *cert = NULL;
	    EVP_PKEY *pubkey = NULL;
	    EVP_MD_CTX *mdctx = NULL;
	    unsigned char *sig = NULL;
	    unsigned char *data = NULL;
	    size_t sig_len, data_len;
	    int ret = 0;

	    OpenSSL_add_all_algorithms();
	    ERR_load_crypto_strings();

	    // Load certificate in DER format
	    fp = fopen(cert_file, "rb");
	    if (!fp) handleErrors();
	    cert = d2i_X509_fp(fp, NULL);
	    fclose(fp);
	    if (!cert) handleErrors();

	    // Extract public key
	    pubkey = X509_get_pubkey(cert);
	    if (!pubkey) handleErrors();

	    // Read signature
	    fp = fopen(sig_file, "rb");
	    if (!fp) handleErrors();
	    fseek(fp, 0, SEEK_END);
	    sig_len = ftell(fp);
	    fseek(fp, 0, SEEK_SET);
	    sig = malloc(sig_len);
	    if (fread(sig, 1, sig_len, fp) != sig_len) handleErrors();
	    fclose(fp);

	    // Read data
	    fp = fopen(data_file, "rb");
	    if (!fp) handleErrors();
	    fseek(fp, 0, SEEK_END);
	    data_len = ftell(fp);
	    fseek(fp, 0, SEEK_SET);
	    data = malloc(data_len);
	    if (fread(data, 1, data_len, fp) != data_len) handleErrors();
	    fclose(fp);

	    // Verify signature
	    mdctx = EVP_MD_CTX_new();
	    if (!mdctx) handleErrors();
	    if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pubkey) <= 0) handleErrors();
	    if (EVP_DigestVerifyUpdate(mdctx, data, data_len) <= 0) handleErrors();
	    ret = EVP_DigestVerifyFinal(mdctx, sig, sig_len);

	    // Clean up
	    EVP_MD_CTX_free(mdctx);
	    EVP_PKEY_free(pubkey);
	    X509_free(cert);
	    free(sig);
	    free(data);

	    return ret == 1;
	}

	int main(int argc, char **argv) {
	    if (argc != 4) {
		fprintf(stderr, "Usage: %s <cert_file> <data_file> <sig_file>\n", argv[0]);
		return 1;
	    }

	    if (verify_signature(argv[1], argv[2], argv[3])) {
		printf("Signature is valid.\n");
	    } else {
		printf("Signature is invalid.\n");
	    }

	    return 0;
	}

Compilamos 

	gcc -o verifySignature verifySignature.c -lssl -lcrypto

Con el ca-cert.der y el user-cert.der en el mismo directorio probamos si verifica la firma:

	./verifySignature <cert_file> <data_file> <sig_file>
	
Modificamos el certificado de usuario para comprobar que verifica bien

	sudo apt install hexedit
	hexedit <data_file>
	
Modificamos algun bit y salimos con Ctrl+x

Verificamos la firma de nuevo
