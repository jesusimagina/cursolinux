# Generar una PKI con Openssl

## Generamos una CA

### 1 - Generamos un openssl.cnf

	[ ca ]
	default_ca = CA_default
	
	[ CA_default ]
	dir               = ./ca
	certs             = $dir/certs
	crl_dir           = $dir/crl
	new_certs_dir     = $dir/newcerts
	database          = $dir/index.txt
	serial            = $dir/serial
	RANDFILE          = $dir/private/.rand
	
	private_key       = $dir/private/ca-key.pem
	certificate       = $dir/certs/ca-cert.pem
	
	default_md        = sha512
	name_opt          = ca_default
	cert_opt          = ca_default
	default_days      = 3650
	preserve          = no
	policy            = policy_strict
	
	[ policy_strict ]
	countryName             = match
	stateOrProvinceName     = match
	organizationName        = match
	organizationalUnitName  = optional
	commonName              = supplied
	emailAddress            = optional
	
	[ req ]
	default_bits        = 4096
	distinguished_name  = req_distinguished_name
	string_mask         = utf8only
	
	default_md          = sha512
	
	[ req_distinguished_name ]
	countryName                     = Country Name (2 letter code)
	stateOrProvinceName             = State or Province Name (full name)
	localityName                    = Locality Name (eg, city)
	0.organizationName              = Organization Name (eg, company)
	organizationalUnitName          = Organizational Unit Name (eg, section)
	commonName                      = Common Name (eg, fully qualified host name)
	emailAddress                    = Email Address
	
	countryName_default             = US
	stateOrProvinceName_default     = California
	localityName_default            = San Francisco
	0.organizationName_default      = My Company
	organizationalUnitName_default  = IT
	commonName_default              = mycompany.com
	emailAddress_default            = admin@mycompany.com
	
	[ v3_ca ]
	subjectKeyIdentifier = hash
	authorityKeyIdentifier = keyid:always,issuer
	basicConstraints = CA:TRUE

### 2 - Crear la estructura de directorios para la CA:
	
	mkdir -p ca/{certs,crl,newcerts,private}
   	touch ca/index.txt
   	echo 1000 > ca/serial


### 2 - Generar la clave privada de la CA:

	openssl ecparam -name brainpoolP512r1 -genkey -noout -out ca/private/ca-key.pem


### 3 - Crear un certificado autofirmado para la CA:

	openssl req -x509 -new -key ca/private/ca-key.pem -sha512 -days 3650 -out ca/certs/ca-cert.pem -config openssl.cnf

Se puede ver el contenido del certificado con 

	openssl x509 -in ca/certs/ca-cert.pem  -text -noout

Y la clave

	openssl pkey -in ca/private/ca-key.pem  -text -noout
	
Esto es lo que se conoce como formato PEM pero ambas pueden pasarse a formato ASN1 que es el estandar

	openssl pkey -in ca/private/ca-key.pem -inform PEM -outform DER -out ca/private/ca-key.der
	openssl x509 -in ca/certs/ca-cert.pem -inform PEM -outform DER -out ca/certs/ca-cert.der
	
## Generamos un certificado de usuario

	mkdir -p users/{certs,private, csr}

### 4 - Generar la clave privada para el usuario
	
	openssl ecparam -name brainpoolP512r1 -genkey -noout -out users/private/user-key.pem


### 5 - Crear una solicitud de firma de certificado (CSR) para el usuario:

	openssl req -new -key users/private/user-key.pem -out users/csr/user.csr -config openssl.cnf


### 6 - Firmar el CSR con la CA para generar el certificado del usuario:

	openssl ca -config openssl.cnf -in users/csr/user.csr -out users/certs/user-cert.pem -keyfile ca/private/ca-key.pem -cert ca/certs/ca-cert.pem
	
### 7 -  Creamos un apliacación que nos verifique la firma

Creamos un fichero verifyCert.c con este contenido.

	#include <openssl/x509.h>
	#include <openssl/pem.h>
	#include <openssl/err.h>
	#include <stdio.h>

	void handle_openssl_error() {
	    ERR_print_errors_fp(stderr);
	    abort();
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
		handle_openssl_error();
	    }

	    return cert;
	}

	int main(int argc, char *argv[]) {

	    if (argc != 3) {
		fprintf(stderr, "Usage: %s <ca_cert> <user_cert>\n", argv[0]);
		return 1;
	    }

	    OpenSSL_add_all_algorithms();
	    ERR_load_BIO_strings();
	    ERR_load_crypto_strings();

	    X509* ca_cert = read_certificate_der(argv[1]);
	    X509* user_cert = read_certificate_der(argv[2]);

	    if (!ca_cert || !user_cert) {
		fprintf(stderr, "Failed to read certificates\n");
		return 1;
	    }

	    EVP_PKEY* ca_public_key = X509_get_pubkey(ca_cert);
	    if (!ca_public_key) {
		handle_openssl_error();
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
	
Se le pasarán dos certificados la CA y el de usuario y verificará la cadena de certificación
 
Compilamos usando el propio gcc enlazando las librerias de openssl

	gcc -o verifyCert verifyCert.c -lssl -lcrypto
	
Probamos 

	./verifyCert ca-cert.der user-cert.der
