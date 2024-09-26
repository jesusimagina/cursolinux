#ifndef _LINUX_CRYPT_H_
#define  _LINUX_CRYPT_H_


int verifyCert(unsigned char* p_ca_cert, int ca_len, unsigned char * p_user_cert, int user_len);

int verify_signature(unsigned char * user_cert_der, int user_cert_der_len, unsigned char *  *data, int data_len,  unsigned char *  *sig, int sig_len);

#endif
