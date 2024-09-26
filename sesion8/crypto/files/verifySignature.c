#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int verify_signature(unsigned char * user_cert_der,int user_cert_der_len, unsigned char *  *data, int data_len,  unsigned char *  *sig, int sig_len) {
   
    FILE *fp;
    X509 *cert = NULL;
    EVP_PKEY *pubkey = NULL;
    EVP_MD_CTX *mdctx = NULL;
    int ret = 0;

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Load certificate in DER format
    cert = read_certificate(user_cert_der, user_cert_der_len);

    // Extract public key
    pubkey = X509_get_pubkey(cert);
    if (!pubkey) handleErrors();

    // Verify signature
    mdctx = EVP_MD_CTX_new();
    if (!mdctx) handleErrors();
    if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha512(), NULL, pubkey) <= 0) handleErrors();
    if (EVP_DigestVerifyUpdate(mdctx, data, data_len) <= 0) handleErrors();
    ret = EVP_DigestVerifyFinal(mdctx,(const char*) sig, sig_len);

    // Clean up
    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pubkey);
    X509_free(cert);

    return ret == 1;
}
