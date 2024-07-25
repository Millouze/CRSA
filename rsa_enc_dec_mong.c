#include "rsa_enc_dec.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

long long int reduce(long long int prod, long long int n, long long int ninv,
                     long long int r) {

  long long int q = prod * ninv;
  long long int m = (q * n) >> 32;
  printf("In reduce m:%lld\n", m);
  return (prod >> 32) + n - m;
}

long long int ndash(long long int n, long long int r) {

  long long int res = 0;
  long long int temp = 0;
  long long int i = 1;

  while (r > 1) {
    if (!(temp % 2)) {
      temp += n;
      res += i;
    }
    temp /= 2;
    r /= 2;
    i *= 2;
  }
  return res;
}

unsigned long long mongmul(unsigned long long a, unsigned long long b,
                           long long int r, long long int n) {
  long long int nd = ndash(n, r);

  unsigned long long tt = a * b;

  unsigned long long temp = (tt * nd) & (r - (unsigned long long)1);

  unsigned long long t = (tt + temp*n) / r;

 //printf("ndash: %lld , tt: %llu , t: %llu , r: %lld\n", nd, tt, t, r);
  return t >= n ? t - n : t;
}

/*Modular exponentiation but with montgomery transform for faster modular exponentiation
 *  and multiplication
 *
 * returns a^b mod m
 *
 */
int mod_exp(int a, int b, long long int n) {

  //printf("In mod_exp; a=%d, b=%d, n=%lld\n", a, b, n);

  long long int R = ((long long int)1 << 32);

  // Pre-compute of -n**-1%R this part is ok
  // printf("R: %lld\n",R);
  //long long int ninv = mod_inv(n, R);

  unsigned long long aprime = ((long long int)a * R) % n;

  unsigned long long ret = aprime;

  //printf("ninv:%lld, aprime:%lld\n", ninv, aprime);

  int order = 31;
  int tb = b;
  // Compute the order of the exponent
  while (!(tb & (1 << 31))) {
    order--;
    tb = tb << 1;
  }
  // printf("binary n: %b\n",(unsigned int)m);
  // printf("b is of order %d\n",order);

  /* Stop if the value of base is 0 */
  if (a == 0) {

    return 0;
  }

  for (int i = order - 1; i >= 0; i--) {
    ret = mongmul(ret, ret, R, n);

    if (b & (1 << i)) {
      // printf("dans le if a i=%d",i);
      ret = mongmul(ret, aprime, R, n);
    }

    //printf("ret is %llu at i = %d\n", ret, i);
  }
  // printf("ret:%lld\n",ret);
  return mongmul(ret, 1, R, n);
}

/* Rsa encryption function
 *
 * encrypts the plaintext data using the provided public key e and modular n
 * needs the length of the message and an int array to store encrypted data
 *
 */
void rsa_enc(char *plaintext, int *ciphertext, size_t len, int e,
             long long int n) {

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
void rsa_dec(char *plaintext, int *ciphertext, size_t len, int d,
             long long int n) {

  for (int i = 0; i < len; i++) {
    plaintext[i] = mod_exp(ciphertext[i], d, n);
  }
}
