#ifndef RSA_ENC_DEC_H
#define RSA_ENC_DEC_H
#include <stddef.h>

void rsa_enc(char *plaintext, int *ciphertext, size_t len, int e, long long int n);

void rsa_dec(char *plaintext, int *ciphertext, size_t len, int d, long long int n);

#endif
