// bignum.hpp
// Author: IS1603 jzr (Bairuo)

#ifndef Bignum_H_
#define Bignum_H_

#include<openssl/bn.h>

#define MAXBYTES 2048

namespace bairuo
{
    BIGNUM* ModExp(const BIGNUM *a, const BIGNUM *b, const BIGNUM *n);
    bool MillerRabin(const BIGNUM *n);
    bool IsPrime(const BIGNUM *num);
    BIGNUM* gcd(const BIGNUM *a, const BIGNUM *b);
    BIGNUM* exgcd(const BIGNUM *a, const BIGNUM *b, BIGNUM *x, BIGNUM *y);
    BIGNUM* modInverse(const BIGNUM *a, const BIGNUM *n);
    BIGNUM *genBigPrime(int length);
}

#endif // Bignum_H_
