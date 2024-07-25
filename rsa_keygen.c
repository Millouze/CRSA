#include "rsa_keygen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int _is_coprime(int a, int b) {

  if (!b) {
    return a == 1;
  }

  return _is_coprime(b, a % b);
}

int genprime() {

  int i, flag = 1;
  srand(time(NULL));
  int n;

  n = rand() % 65536;
  while (flag) {

    flag = 0;

    if (n == 0 || n == 1)
      flag = 1;

    for (i = 2; i <= n / 2; ++i) {
      // if n is divisible by i, then n is not prime
      // change flag to 1 for non-prime number
      if (n % i == 0) {
        flag = 1;
        break;
      }
    }
    if (i >= n / 2) {
      return n;
    }
    n--;
  }
  return n;
}

/**
 *Finds the multiplicative inverses of the two numbers
 */
void _extended_euclid(int a, int b, int *x, int *y) {

  /* If second number is zero */
  if (!a) {
    *x = 0;
    *y = 1;
    return;
  }

  int _x, _y;

  /* Find the coefficients recursively */
  _extended_euclid(b % a, a, &_x, &_y);

  /* Update the coefficients */
  *x = _y - (b / a) * _x;
  *y = _x;
}

/**
 *Returns the multiplicative inverse of a under base m
 */
int _mod_inv(int a, int m) {

  int inv_a, inv_m;

  /* Compute the coefficients using extended euclidean algorithm */
  _extended_euclid(a, m, &inv_a, &inv_m);
  /* If the inverse is negative convert it to positive under m */
  if (inv_a < 0) {
    inv_a = (m + inv_a) % m;
  }

  return inv_a;
}

static long long int mod_inv(long long int ra, long long int rb) {
  //printf("ra :%lld, rb: %lld\n", ra, rb);
  long long int rc, sa = 1, sb = 0, sc, i = 0;
  if (rb > 1)
    do {
      rc = ra % rb;
      sc = sa - (ra / rb) * sb;
      sa = sb, sb = sc;
      ra = rb, rb = rc;
    } while (++i, rc);
  sa *= (i *= ra == 1) != 0;
  sa += (i & 1) * sb;
  return sa;
}

/*Fills the adresses passed as arg with:
 *  e with the public key
 *  d with the private key
 *  n with the modulus
 *  Returns nothing
 */
void naive_rsa_keygen(int *e, int *d, long long int *n) {
  // First step is to generate 2 32 bits prime numbers
  int p = 251; // genprime();
  int q = genprime();

  // Computing value of n
  *n = p * q;

  //printf("p: %d, q:%d, n:%lld\n", p, q, *n);

  // Finding the value of phi(n)
  long long int phi_n = (p - 1) * (q - 1);

  // prime e value
  *e = 65537;
  //printf("phi_n: %lld\n",phi_n);
  // Find the multiplicative inverse of e modulo phi_n
  *d = mod_inv(*e, phi_n);

  return;
}
