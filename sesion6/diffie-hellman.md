
# Generación de un diffie-hellman

Tenemos el siguiente fichero dh.c

	#include <stdio.h>
	#include <openssl/evp.h>
	#include <openssl/pem.h>
	#include <openssl/x509.h>
	#include <openssl/bio.h>
	#include <openssl/dh.h>
	#include <openssl/err.h>
	#include <openssl/sha.h>

	void handle_errors() {
	    ERR_print_errors_fp(stderr);
	    abort();
	}

	EVP_PKEY *load_key(const char *file) {
	    FILE *fp = fopen(file, "rb");
	    if (!fp) handle_errors();

	    EVP_PKEY *key = d2i_PrivateKey_fp(fp, NULL);
	    fclose(fp);
	    if (!key) handle_errors();

	    return key;
	}

	X509 *load_cert(const char *file) {
	    FILE *fp = fopen(file, "rb");
	    if (!fp) handle_errors();

	    X509 *cert = d2i_X509_fp(fp, NULL);
	    fclose(fp);
	    if (!cert) handle_errors();

	    return cert;
	}

	EVP_PKEY *get_pubkey_from_cert(X509 *cert) {
	    EVP_PKEY *pubkey = X509_get_pubkey(cert);
	    if (!pubkey) handle_errors();
	    return pubkey;
	}

	int main(int argc, char *argv[]) {
	    if (argc != 4) {
		fprintf(stderr, "Usage: %s <cert1.der> <key2.der> outputKey\n", argv[0]);
		return 1;
	    }

	    OpenSSL_add_all_algorithms();
	    ERR_load_crypto_strings();

	    const char *cert1_file = argv[1];
	    const char *key2_file = argv[2];
	    const char *output_file = argv[3];

	    X509 *cert1 = load_cert(cert1_file);
	    EVP_PKEY *privkey2 = load_key(key2_file);
	    EVP_PKEY *pubkey1 = get_pubkey_from_cert(cert1);

	    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey2, NULL);
	    if (!ctx) handle_errors();

	    if (EVP_PKEY_derive_init(ctx) <= 0) handle_errors();
	    if (EVP_PKEY_derive_set_peer(ctx, pubkey1) <= 0) handle_errors();

	    size_t secret_len;
	    if (EVP_PKEY_derive(ctx, NULL, &secret_len) <= 0) handle_errors();

	    unsigned char *secret = OPENSSL_malloc(secret_len);
	    if (!secret) handle_errors();

	    if (EVP_PKEY_derive(ctx, secret, &secret_len) <= 0) handle_errors();

	    unsigned char hash[SHA256_DIGEST_LENGTH];
	    if (!SHA256(secret, secret_len, hash)) handle_errors();

	    FILE *out_fp = fopen(output_file, "wb");
	    if (!out_fp) handle_errors();

	    if (fwrite(hash, 1, SHA256_DIGEST_LENGTH, out_fp) != SHA256_DIGEST_LENGTH) handle_errors();

	    fclose(out_fp);

	    OPENSSL_free(secret);
	    EVP_PKEY_CTX_free(ctx);
	    EVP_PKEY_free(pubkey1);
	    EVP_PKEY_free(privkey2);
	    X509_free(cert1);
	    EVP_cleanup();
	    ERR_free_strings();

	    return 0;
	}
	
Compilamos usando el propio gcc enlazando las librerias de openssl

	gcc -o dh dh.c -lssl -lcrypto
	
Probamos 

	./dh user1-cert.der user2-cert.der
