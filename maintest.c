#include "rsa_enc_dec.h"
#include "rsa_keygen.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  int d, e;
  long long int n;

  char plaintext[256];
  int ciphertext[256];
  char decplaintext[256];

  size_t len;

  naive_rsa_keygen(&e, &d, &n);

  printf("generated keys e: %d, d: %d, and modulo %lld\n", e, d, n);

  sprintf(plaintext, "Montgomery transform is done");

  len = strlen(plaintext);

  printf("message to encrypt: %s \n", plaintext);
  rsa_enc(plaintext, ciphertext, len, e, n);

  printf("cipher %s\n",(char*)ciphertext);
  rsa_dec(decplaintext, ciphertext, len, d, n);

  printf("decrypted message: %s \n", decplaintext);

  return 0;
}
