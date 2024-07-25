#include "rsa_enc_dec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

/*Modular exponentiation without montgomery transform because I don't get it yet
 * T-T
 *
 * returns a^b mod m
 *
 */
int mod_exp(int a, int b, long long int m) {

 // printf("In mod_exp; a=%d, b=%d, m=%lld\n",a,b,m);
  
  long long int ret = a;
  
  int order = 31;
  int tb =b;
  //Calculate the order of the exponent
  while (!(tb& (1<<31))) {
    order--;
    tb=tb<<1;
  }
  //printf("binary m: %b\n",(unsigned int)m);
  //printf("b is of order %d\n",order);


  /* Stop if the value of base is 0 */
  if (a == 0) {

    return 0;
  }

  for (int i = order-1;i>=0 ;i-- ) {
    ret = (ret *ret) % m;
    
    if(b& (1<<i)){
      //printf("dans le if a i=%d",i);
      ret = (ret*a) % m;
    }
    
      //printf("ret is %lld at i = %d\n",ret,i);
      
  }
 // printf("ret:%lld\n",ret);
  return ret;
}

/* Rsa encryption function
 *
 * encrypts the plaintext data using the provided public key e and modular n
 * needs the length of the message and an int array to store encrypted data
 *
 */
void rsa_enc(char *plaintext, int *ciphertext, size_t len, int e, long long int n) {

  for (int i = 0; i < len; i++) {
    ciphertext[i] = mod_exp(plaintext[i], e, n);
  }
}

/* Rsa decryption function
 *
 * encrypts the plaintext data using the provided public key e and modular n
 * needs the length of the message and an int array to store encrypted data
 *
 */
void rsa_dec(char *plaintext, int *ciphertext, size_t len, int d, long long int n) {

  for (int i = 0; i < len; i++) {
    plaintext[i] = mod_exp(ciphertext[i], d, n);
  }
}
