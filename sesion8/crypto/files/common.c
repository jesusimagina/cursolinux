#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "common.h"

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

unsigned char* file_to_unsigned_char(const char* filename, int* size) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    long len = 0;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = malloc(len);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    *size = len;

    fread(buffer, 1, len, file);
    fclose(file);
    return buffer;
}

X509* read_certificate_der(const char* filename) {

    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    X509* cert = d2i_X509_fp(file, NULL);
    fclose(file);

    if (!cert) {
        handleErrors();
    }

    return cert;
}

X509* read_certificate(unsigned char* pcert, int length) {

	const unsigned char* p = pcert;
    X509* cert = d2i_X509(NULL, &p, length);

    if (!cert) {
        handleErrors();
    }

    return cert;
}
