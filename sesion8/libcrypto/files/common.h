#ifndef _COMMON_H_
#define _COMMON_H_

void handleErrors(void);

unsigned char* file_to_unsigned_char(const char* filename, int* size);

X509* read_certificate_der(const char* filename);
X509* read_certificate(unsigned char* pcert, int length);

#endif
