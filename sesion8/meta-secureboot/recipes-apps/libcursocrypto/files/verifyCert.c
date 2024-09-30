#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>

#include "common.h"
#include "linuxCrypt.h"


int verifyCert(unsigned char* p_ca_cert, int ca_len, unsigned char * p_user_cert, int user_len) {

    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    
    X509* ca_cert = read_certificate(p_ca_cert, ca_len);
    X509* user_cert =read_certificate(p_user_cert, user_len);
    
    if (!ca_cert || !user_cert) {
        fprintf(stderr, "Failed to read certificates\n");
        return 1;
    }

    EVP_PKEY* ca_public_key = X509_get_pubkey(ca_cert);
    if (!ca_public_key) {
        handleErrors();
    }

    int verify_result = X509_verify(user_cert, ca_public_key);
    if (verify_result == 1) {
        printf("Certificate verification successful.\n");
    } else {
        printf("Certificate verification failed.\n");
    }

    EVP_PKEY_free(ca_public_key);
    X509_free(ca_cert);
    X509_free(user_cert);
    EVP_cleanup();
    ERR_free_strings();

    return verify_result == 1 ? 0 : 1;
}
